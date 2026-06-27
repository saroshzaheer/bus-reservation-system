import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path


class BackendSmokeTests(unittest.TestCase):
    def setUp(self):
        self.project_dir = Path(__file__).resolve().parents[1]
        self.binary_path = self.project_dir / "output" / "main.exe"

    def test_cpp_backend_compiles_and_accepts_signup(self):
        subprocess.run(
            ["g++", "-std=c++17", "-O2", "-o", str(self.binary_path), str(self.project_dir / "main.cpp")],
            cwd=self.project_dir,
            check=True,
            capture_output=True,
            text=True,
        )

        result = subprocess.run(
            [
                str(self.binary_path),
                "signup",
                "testuser",
                "pass123",
                "pass123",
                "03001234567",
                "Test User",
                "test@example.com",
                "Mother",
                "City",
            ],
            cwd=self.project_dir,
            check=False,
            capture_output=True,
            text=True,
        )

        self.assertEqual(result.returncode, 0, msg=result.stderr or result.stdout)
        self.assertIn('"success":true', result.stdout.lower())


if __name__ == "__main__":
    unittest.main()
