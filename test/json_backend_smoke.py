#!/usr/bin/env python3
"""Smoke checks for internal JSON writer output.

English: validates nested container state, control-char escaping, and invalid UTF-8 handling.
Deutsch: validiert verschachtelten Container-State, Control-Char-Escaping und ungültiges UTF-8.
"""

import json
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CPP = ROOT / "test" / "json_backend_smoke_tmp.cc"
BIN = ROOT / "test" / "json_backend_smoke_tmp"

CPP.write_text(
    r'''
#include "src/json/json_backend.h"
#include <iostream>
#include <string>

static std::string emit_nested_map_case(const unsigned char *payload, size_t len) {
    yajl_gen g = yajl_gen_alloc(nullptr);
    const unsigned char *buf = nullptr;
    size_t out_len = 0;

    yajl_gen_map_open(g);
    yajl_gen_string(g, (const unsigned char*)"k", 1);
    yajl_gen_map_open(g);
    yajl_gen_string(g, (const unsigned char*)"nested", 6);
    yajl_gen_string(g, payload, len);
    yajl_gen_map_close(g);
    yajl_gen_string(g, (const unsigned char*)"next", 4);
    yajl_gen_integer(g, 2);
    yajl_gen_map_close(g);

    yajl_gen_get_buf(g, &buf, &out_len);
    std::string out((const char*)buf, out_len);
    yajl_gen_free(g);
    return out;
}

int main() {
    const unsigned char valid_utf8[] = {'A', 0x01, 'B'};
    const unsigned char invalid_utf8[] = {0xC3, 0x28};

    std::cout << emit_nested_map_case(valid_utf8, sizeof(valid_utf8)) << "\n";
    std::cout << emit_nested_map_case(invalid_utf8, sizeof(invalid_utf8)) << "\n";

    yajl_gen g = yajl_gen_alloc(nullptr);
    const unsigned char *buf = nullptr;
    size_t out_len = 0;
    yajl_gen_map_open(g);
    yajl_gen_string(g, (const unsigned char*)"k", 1);
    yajl_gen_array_open(g);
    yajl_gen_integer(g, 1);
    yajl_gen_integer(g, 2);
    yajl_gen_array_close(g);
    yajl_gen_string(g, (const unsigned char*)"next", 4);
    yajl_gen_integer(g, 3);
    yajl_gen_map_close(g);
    yajl_gen_get_buf(g, &buf, &out_len);
    std::cout << std::string((const char*)buf, out_len) << "\n";
    yajl_gen_free(g);

    g = yajl_gen_alloc(nullptr);
    yajl_gen_array_open(g);
    yajl_gen_map_open(g);
    yajl_gen_string(g, (const unsigned char*)"k", 1);
    yajl_gen_map_open(g);
    yajl_gen_string(g, (const unsigned char*)"x", 1);
    yajl_gen_array_open(g);
    yajl_gen_integer(g, 1);
    yajl_gen_integer(g, 2);
    yajl_gen_array_close(g);
    yajl_gen_map_close(g);
    yajl_gen_map_close(g);
    yajl_gen_array_close(g);
    yajl_gen_get_buf(g, &buf, &out_len);
    std::cout << std::string((const char*)buf, out_len) << "\n";
    yajl_gen_free(g);

    return 0;
}
'''
)

subprocess.run([
    "g++", "-std=c++17", "-I", str(ROOT), "-o", str(BIN), str(CPP), str(ROOT / "src/json/json_backend.cc")
], check=True)

lines = subprocess.check_output([str(BIN)], text=True).splitlines()
assert len(lines) == 4

obj0 = json.loads(lines[0])
assert obj0["k"]["nested"] == "A\x01B"
assert obj0["next"] == 2

obj1 = json.loads(lines[1])
assert obj1["k"]["nested"] == "\u00c3("

obj2 = json.loads(lines[2])
assert obj2 == {"k": [1, 2], "next": 3}

obj3 = json.loads(lines[3])
assert obj3 == [{"k": {"x": [1, 2]}}]

print("json_backend_smoke: ok")
