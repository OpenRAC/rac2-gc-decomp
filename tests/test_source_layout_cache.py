"""Exact-content span memo invariants; no repository corpus or compiler needed."""
from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
import source_layout as tool


class SourceSpanCacheTests(unittest.TestCase):
    def setUp(self):
        tool._function_span_cached.cache_clear()
        self.addCleanup(tool._function_span_cached.cache_clear)
        self.original = tool._function_span_cached.__wrapped__

    def test_equal_immutable_bytes_share_hit(self):
        data = b"int FUN_A(void) { return 1; }\n"
        equal = bytes(bytearray(data))
        self.assertIsNot(data, equal)
        self.assertEqual(tool.function_span(data, "FUN_A"), self.original(data, "FUN_A"))
        self.assertEqual(tool.function_span(equal, "FUN_A"), self.original(data, "FUN_A"))
        self.assertEqual(tool._function_span_cached.cache_info().hits, 1)
        self.assertEqual(tool._function_span_cached.cache_info().misses, 1)

    def test_content_and_name_are_both_exact_keys(self):
        data = b"void FUN_A(void) {}\nvoid FUN_B(void) {}\n"
        moved = b"/* moves indexes */\n" + data
        self.assertNotEqual(tool.function_span(data, "FUN_A"), tool.function_span(moved, "FUN_A"))
        self.assertNotEqual(tool.function_span(data, "FUN_A"), tool.function_span(data, "FUN_B"))
        self.assertEqual(tool._function_span_cached.cache_info().currsize, 3)

    def test_mutable_buffer_and_view_always_use_current_content(self):
        for factory in (lambda value: value, memoryview):
            with self.subTest(factory=factory):
                data = bytearray(b"void FUN_A(void) {}\n")
                value = factory(data)
                self.assertEqual(tool.function_span(value, "FUN_A"), self.original(value, "FUN_A"))
                data[16:18] = b"  "
                with self.assertRaises(ValueError):
                    tool.function_span(value, "FUN_A")
                self.assertEqual(tool._function_span_cached.cache_info().currsize, 0)

    def test_braces_comments_strings_char_escapes_and_crlf(self):
        data = (b"int FUN_A(void) { /* } */\r\n"
                b" char c='}'; const char *s=\"{\\\"}\"; // }\r\n"
                b" if(c) { return 1; } return 0;\r\n}\r\n")
        expected = self.original(data, "FUN_A")
        self.assertEqual(tool.function_span(data, "FUN_A"), expected)
        self.assertEqual(tool.function_span(data, "FUN_A"), expected)

    def test_refusals_keep_exact_exception_type_message_and_are_not_cached(self):
        for data, name in ((b"void FUN_B(void) {}\n", "FUN_A"),
                           (b"void FUN_A(void) {}\nvoid FUN_A(void) {}\n", "FUN_A"),
                           (b"void FUN_A(void) {", "FUN_A"), (None, "FUN_A"),
                           (b"void FUN_A(void) {}", None), (b"void FUN_A(void) {}", "\ud800")):
            with self.subTest(data=data, name=name):
                try:
                    self.original(data, name)
                except Exception as expected:
                    kind, message = type(expected), str(expected)
                for _ in range(2):
                    with self.assertRaises(kind) as got:
                        tool.function_span(data, name)
                    self.assertEqual(str(got.exception), message)
                self.assertEqual(tool._function_span_cached.cache_info().currsize, 0)

    def test_unhashable_bytes_subclass_retains_uncached_api(self):
        class CustomBytes(bytes):
            __hash__ = None
        class CustomName(str):
            __hash__ = None
        data = CustomBytes(b"void FUN_A(void) {}")
        self.assertEqual(tool.function_span(data, "FUN_A"), self.original(data, "FUN_A"))
        name = CustomName("FUN_A")
        self.assertEqual(tool.function_span(bytes(data), name), self.original(bytes(data), name))
        self.assertEqual(tool._function_span_cached.cache_info().currsize, 0)

    def test_capacity_evicts_and_clear_releases_without_altering_result(self):
        capacity = tool._function_span_cached.cache_info().maxsize
        self.assertEqual(capacity, 16000)
        first = b"void FUN_A(void) { /* original */ }\n"
        expected = tool.function_span(first, "FUN_A")
        for index in range(capacity):
            tool.function_span(f"void FUN_A(void) {{ /* {index} */ }}\n".encode(), "FUN_A")
        self.assertEqual(tool._function_span_cached.cache_info().currsize, capacity)
        misses = tool._function_span_cached.cache_info().misses
        self.assertEqual(tool.function_span(first, "FUN_A"), expected)
        self.assertEqual(tool._function_span_cached.cache_info().misses, misses + 1)
        tool._function_span_cached.cache_clear()
        self.assertEqual(tool._function_span_cached.cache_info().currsize, 0)


if __name__ == "__main__":
    unittest.main()
