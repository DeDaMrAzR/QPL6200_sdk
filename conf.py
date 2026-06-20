from datetime import datetime
from recommonmark.transform import AutoStructify
import warnings
from recommonmark.parser import CommonMarkParser

# Suppress the specific warning
warnings.filterwarnings("ignore", message="Container node skipped: type=document")


class CustomCommonMarkParser(CommonMarkParser):
    def visit_document(self, mdnode):
        try:
            super().visit_document(mdnode)
        except UserWarning:
            pass


source_parsers = {
    '.md': CustomCommonMarkParser,
}

# -- Path setup --------------------------------------------------------------
# -- Project information -----------------------------------------------------
project = 'QPG6200 IoT SDK'
copyright = '2025, Qorvo'
author = 'Qorvo'
# X.Y.Z version
version = "2.0.0"

# -- General configuration ---------------------------------------------------
# add extensions directory
try:
    from pathlib import Path
    import sys
    sys.path.append(str(Path('extensions').resolve()))
except (ModuleNotFoundError, ImportError):
    print("WARNING: EXTENSION LOADING FAILED. String replacement for ReCommonmark will not work. Expect broken links in .md files")

extensions = [
    'recommonmark',
    'sphinx_markdown_tables',
    'sphinxemoji.sphinxemoji',
    'sphinxcontrib.youtube',
    "sphinx.ext.autodoc",
    "sphinx.ext.viewcode",
    "sphinx.ext.todo",
    "sphinx.ext.autosummary",
    "sphinx.ext.napoleon",
    "sphinx.ext.autosectionlabel",
    "sphinx_panels",
    "regex_links",
]
sphinxemoji_style = 'twemoji'


# -- Template configuration ---------------------------------------------------
# https://sphinx-rtd-theme.readthedocs.io/en/stable/configuring.html
html_theme_options = {
    # 'analytics_id': 'G-XXXXXXXXXX',  #  Provided by Google in your dashboard
    # 'analytics_anonymize_ip': False,
    # 'logo_only': False,
    'prev_next_buttons_location': 'both',
    # 'style_external_links': True,
    # 'vcs_pageview_mode': '',
    # 'style_nav_header_background': '#181616',
    # 'flyout_display': 'attached',
    # 'version_selector': True, # needs a configured addon: https://docs.readthedocs.io/en/stable/addons.html
    # 'language_selector': True,
    # Toc options
    # 'collapse_navigation': False,
    # 'sticky_navigation': False,
    # 'navigation_depth': -1,
    # 'includehidden': True,
    # 'titles_only': False
}

# Enable markdown
source_suffix = ['.rst', '.md']

templates_path = ['_templates']
exclude_patterns = []

# Path to look for the Markdown files
master_doc = 'index'

language = "en"

# Use numbers for figures.
numfig = True

# Ignore "duplicated label" error.
# We have same labels in multiple files, because we have templates for many elements of the applications, eg. shared modules
suppress_warnings = ['autosectionlabel.*']

# -- Options for HTML output -------------------------------------------------
html_theme = 'sphinx_rtd_theme'
html_logo = 'sphinx/qorvo_logo.png'


# -- Add CSS overrides -------------------------------------------------
html_static_path = ['_static']
html_css_files = ['css/overrides.css']

exclude_patterns = ['_build', 'sphinx-venv', 'Components', '.python_venv', 'venv']

# -- Extension configuration -------------------------------------------------


def setup(app):
    app.add_config_value('recommonmark_config', {
        'auto_toc_tree_section': 'Contents',
    }, True)
    app.add_transform(AutoStructify)
