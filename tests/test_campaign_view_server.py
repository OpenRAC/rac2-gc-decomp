"""Private review serving must not expose files or writable endpoints."""
from pathlib import Path
import http.client
import sys
import tempfile
import threading
import unittest
from urllib.parse import urlsplit

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
import campaign_view_server


class ReviewServerTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.path = Path(self.temp.name) / "private.html"
        self.path.write_bytes(b"<!doctype html><title>Private review</title>")
        self.server, self.url = campaign_view_server.create_server(self.path)
        self.thread = threading.Thread(target=self.server.serve_forever, daemon=True)
        self.thread.start()
        self.addCleanup(self.stop)

    def stop(self):
        self.server.shutdown()
        self.server.server_close()
        self.thread.join(timeout=3)

    def request(self, method="GET", path=None, headers=None):
        parsed = urlsplit(self.url)
        conn = http.client.HTTPConnection(parsed.hostname, parsed.port, timeout=3)
        self.addCleanup(conn.close)
        conn.request(method, path or parsed.path, headers=headers or {})
        result = conn.getresponse()
        return result.status, dict(result.getheaders()), result.read()

    def test_only_loopback_and_exact_private_route(self):
        self.assertEqual(self.server.server_address[0], "127.0.0.1")
        status, headers, body = self.request()
        self.assertEqual(status, 200)
        self.assertEqual(body, self.path.read_bytes())
        self.assertEqual(headers["Cache-Control"], "no-store")
        self.assertIn("default-src 'none'", headers["Content-Security-Policy"])
        for route in ("/", "/private.html", "/../private.html", urlsplit(self.url).path + "?file=private.html"):
            self.assertEqual(self.request(path=route)[0], 404)

    def test_foreign_hosts_origins_and_writes_refused(self):
        self.assertEqual(self.request(headers={"Host": "attacker.example"})[0], 403)
        self.assertEqual(self.request(headers={"Origin": "https://attacker.example"})[0], 403)
        for method in ("POST", "PUT", "PATCH", "DELETE"):
            self.assertEqual(self.request(method=method)[0], 405)
        self.assertEqual(self.path.read_bytes(), b"<!doctype html><title>Private review</title>")

    def test_head_has_no_body(self):
        status, headers, body = self.request(method="HEAD")
        self.assertEqual(status, 200)
        self.assertEqual(body, b"")
        self.assertEqual(int(headers["Content-Length"]), len(self.path.read_bytes()))


if __name__ == "__main__":
    unittest.main()
