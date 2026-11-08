#!/usr/bin/env python3
"""Check a TSP tour with the TSP Checker API (single file, stdlib only)."""

import argparse
import json
import os
import sys
import urllib.error
import urllib.request
import uuid

DEFAULT_SERVER = "http://10.0.22.151"
TIMEOUT = 10

# the API lives on the university network: connect directly,
# never through the system HTTP proxy
OPENER = urllib.request.build_opener(urllib.request.ProxyHandler({}))


def build_body(instance_path, solution_path):
    boundary = "----tspcheck" + uuid.uuid4().hex
    body = b""
    for field, path in (("instance", instance_path), ("solution", solution_path)):
        head = (
            f"--{boundary}\r\n"
            f'Content-Disposition: form-data; name="{field}"; '
            f'filename="{os.path.basename(path)}"\r\n'
            "Content-Type: application/octet-stream\r\n\r\n"
        ).encode()
        with open(path, "rb") as f:
            body += head + f.read() + b"\r\n"
    body += f"--{boundary}--\r\n".encode()
    return body, f"multipart/form-data; boundary={boundary}"


def unreachable(check_url):
    print(f"Error: could not reach the TSP Checker API at {check_url}.")
    print("The API is only reachable from the university network:")
    print("make sure you are connected to the university VPN, then try again.")


def main():
    parser = argparse.ArgumentParser(
        description="Check a TSP tour with the TSP Checker API."
    )
    parser.add_argument("instance", help="path to the .tsp instance file")
    parser.add_argument("solution", help="path to the .sol tour file")
    parser.add_argument("--server", default=DEFAULT_SERVER,
                        help="API base URL (default: %(default)s)")
    args = parser.parse_args()

    check_url = args.server.rstrip("/") + "/check"

    for path in (args.instance, args.solution):
        if not os.path.isfile(path):
            print(f"Error: file {path} does not exists.")
            return 1

    body, content_type = build_body(args.instance, args.solution)
    request = urllib.request.Request(check_url, data=body, method="POST",
                                     headers={"Content-Type": content_type})

    try:
        response = OPENER.open(request, timeout=TIMEOUT)
    except urllib.error.HTTPError as e:
        try:
            payload = json.loads(e.read())
            status = e.code
        except ValueError:
            print(f"Error: unexpected non-JSON response (HTTP {e.code}) "
                  f"from {check_url}.")
            return 1
    except Exception:
        unreachable(check_url)
        return 2
    else:
        with response:
            payload = json.loads(response.read())
            status = response.status

    if status == 200:
        sys.stdout.write(payload["output"])
        return 0
    if status == 422:
        sys.stdout.write(payload["output"])
        return 1
    print(f"Error: {payload.get('detail', f'HTTP {status}')}")
    return 1


if __name__ == "__main__":
    sys.exit(main())