# Configuration file for the Sphinx documentation builder.
#

# Configure the package's path
import os
import sys
import datetime
from subprocess import check_output

sys.path.insert(0, os.path.abspath("../src/"))
sys.path.insert(0, os.path.abspath(".."))

# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = "Radio Control Console"
copyright = f"{datetime.datetime.now().year}, Qorvo"
author = "Qorvo"
release = check_output(["git", "branch", "--show-current"]).strip().decode("UTF-8")
if release not in ("main", "master"):
    _hash = check_output(["git", "rev-parse", "HEAD"]).strip().decode("UTF-8")
    release += f" | {_hash}"

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    "sphinx.ext.autodoc",
    "sphinx.ext.viewcode",
    "sphinx.ext.todo",
    "sphinx.ext.autosummary",
    "sphinx.ext.napoleon",
    "sphinx.ext.autosectionlabel",
    "sphinx_panels",
]

templates_path = ["_templates"]
exclude_patterns = ["_build", "Thumbs.db", ".DS_Store"]

gitlab_url = "https://gitlab.com/qorvo/wcon/qmodules/rcc"

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output
# https://sphinx-rtd-theme.readthedocs.io/en/stable/configuring.html
html_theme = "sphinx_rtd_theme"
html_static_path = ["_static"]
html_logo = "assets/Qorvo Logo_ID_Blk_TM_RGB.png"
html_theme_options = {
    "display_version": True,
    "collapse_navigation": False,
    "sticky_navigation": True,
    "navigation_depth": 3,
    "style_nav_header_background": "#00a0df",  # User Qorvo blue
}
