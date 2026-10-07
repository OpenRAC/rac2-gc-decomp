"""Serve one already generated private review on the loopback interface."""
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
import secrets
from urllib.parse import urlsplit
import webbrowser


def create_server(review: Path, port: int = 0):
    """Return (server, URL); never expose a directory or a writable endpoint."""
    if type(port) is not int or not 0 <= port <= 65535:
        raise ValueError("Review port must be between 0 and 65535")
    payload = Path(review).read_bytes()
    route = "/review/" + secrets.token_urlsafe(32)

    class Handler(BaseHTTPRequestHandler):
        def log_message(self, *_):
            pass

        def _respond(self, head=False):
            hosts = {"127.0.0.1:" + str(self.server.server_port),
                     "localhost:" + str(self.server.server_port)}
            if self.headers.get("Host", "").lower() not in hosts:
                self.send_error(403, "Invalid review host")
                return
            origin = self.headers.get("Origin")
            if origin and origin not in {"http://" + h for h in hosts}:
                self.send_error(403, "Cross-origin review access refused")
                return
            parsed = urlsplit(self.path)
            if parsed.path != route or parsed.query:
                self.send_error(404)
                return
            self.send_response(200)
            self.send_header("Content-Type", "text/html; charset=utf-8")
            self.send_header("Content-Length", str(len(payload)))
            self.send_header("Cache-Control", "no-store")
            self.send_header("X-Content-Type-Options", "nosniff")
            self.send_header("Referrer-Policy", "no-referrer")
            self.send_header("Content-Security-Policy", "default-src 'none'; script-src 'unsafe-inline'; style-src 'unsafe-inline'; img-src data:; base-uri 'none'; form-action 'none'; frame-ancestors 'none'")
            self.end_headers()
            if not head:
                self.wfile.write(payload)

        def do_GET(self):
            self._respond()

        def do_HEAD(self):
            self._respond(head=True)

        def do_POST(self):
            self.send_error(405, "Review is read-only")

        do_PUT = do_POST
        do_PATCH = do_POST
        do_DELETE = do_POST

    server = ThreadingHTTPServer(("127.0.0.1", port), Handler)
    server.daemon_threads = True
    return server, "http://127.0.0.1:" + str(server.server_port) + route


def serve_review(review: Path, port: int = 0, open_browser: bool = False):
    """Block until interrupted; opening the user's browser is explicitly opt-in."""
    server, url = create_server(review, port)
    print("Private read-only review: " + url, flush=True)
    print("Press Ctrl+C to stop the review server.", flush=True)
    try:
        if open_browser:
            webbrowser.open(url)
        server.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        server.server_close()
