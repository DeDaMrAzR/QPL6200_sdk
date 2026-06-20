import re
from sphinx.application import Sphinx

def replace_text(app, docname, source):
    replacements = [
        # Mitigate recommonmark bug where #header is not transformed to html.
        (r'\.md#', '.html#'),
        (r'\.rst#', '.html#'),
        # Remove extensions if no #header.
        # Recommended for recommonmark to evaluate links in md files.
        (r'\.md\)', ')'),
        (r'\.rst\)', ')'),
    ]
    # Replace and prepare the log
    log_entries = []
    lines = source[0].split('\n')
    for i, line in enumerate(lines):
        original_line = line
        for pattern, replacement in replacements:
            line = re.sub(pattern, replacement, line)
        if line != original_line:
            log_entries.append(f"Line {i+1} ¤ {original_line} ¤ {line}") # delimiter for CSV import
            lines[i] = line

    source[0] = '\n'.join(lines)

    # Log substitutions to file
    with open('substitution_log.txt', 'a') as log_file:
        for entry in log_entries:
            log_file.write(entry + '\n')

def setup(app: Sphinx):
    app.connect('source-read', replace_text)
