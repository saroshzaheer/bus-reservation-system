import re
import tokenize
import io
from pathlib import Path

def remove_python_comments(source_code):
    """Remove comments and docstrings from Python source code."""
    pattern = r'(\"\"\"[\s\S]*?\"\"\"|\'\'\'[\s\S]*?\'\'\'|"[^"\\]*(?:\\.[^"\\]*)*"|\'[^\'\\]*(?:\\.[^\'\\]*)*\')|(#.*)'
    
    def replacer(match):
        if match.group(2):
            return "" # It's a comment
        else:
            return match.group(1) # It's a string
            
    return re.sub(pattern, replacer, source_code)

def remove_cpp_comments(text):
    """Remove C-style comments (// and /* */) while preserving strings."""
    pattern = r'("[^"\\]*(?:\\.[^"\\]*)*"|\'[^\'\\]*(?:\\.[^\'\\]*)*\')|(/\*[^*]*\*+(?:[^/*][^*]*\*+)*|//.*)'
    
    def replacer(match):
        if match.group(2):
            return "" # It's a comment
        else:
            return match.group(1) # It's a string
            
    return re.sub(pattern, replacer, text)

def remove_html_comments(text):
    """Remove HTML comments <!-- -->."""
    pattern = r'<!--[\s\S]*?-->'
    return re.sub(pattern, "", text)

def remove_block_comments_safe(text):
    """Remove C-style block comments (/* */) while preserving strings.
    Does NOT remove // comments to avoid breaking URLs in HTML text."""
    pattern = r'("[^"\\]*(?:\\.[^"\\]*)*"|\'[^\'\\]*(?:\\.[^\'\\]*)*\')|(/\*[\s\S]*?\*/)'
    
    def replacer(match):
        if match.group(2):
            return "" # It's a block comment
        else:
            return match.group(1) # It's a string
            
    return re.sub(pattern, replacer, text)

def main():
    base_dir = Path('.')
    
    # 1. app.py (Python)
    # Already done, but no harm repeating
    
    # 2. main.cpp (C++)
    # Already done
        
    # 3. 1.html (HTML)
    html_file = base_dir / "1.html"
    if html_file.exists():
        print(f"Processing {html_file}...")
        content = html_file.read_text(encoding='utf-8')
        # First remove HTML comments if any left
        content = remove_html_comments(content)
        # Now remove block comments /* */ (CSS/JS)
        content = remove_block_comments_safe(content)
        
        # Clean up empty lines
        content = re.sub(r'\n\s*\n', '\n', content)
        html_file.write_text(content, encoding='utf-8')

if __name__ == "__main__":
    main()
