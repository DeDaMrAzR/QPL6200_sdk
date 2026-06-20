# Minimal makefile for Sphinx documentation
#

# You can set these variables from the command line.
SPHINXOPTS    =
SPHINXBUILD   = sphinx-build
SOURCEDIR     = .
BUILDDIR      = _build

# User-friendly check for sphinx-build
HAVE_SPHINX := $(shell if which $(SPHINXBUILD) >/dev/null 2>&1; then echo 1; else echo 0; fi)

ifeq ($(HAVE_SPHINX),0)

.DEFAULT:
	@echo "The '$(SPHINXBUILD)' command was not found. Make sure you have"
	@echo "Sphinx installed and in PATH, or set the SPHINXBUILD make"
	@echo "variable to point to the full path of the '$(SPHINXBUILD)'"
	@echo "executable."
	@echo
	@echo "You can install it in a venv like this:"
	@echo
	@echo "    python3 -m venv sphinx-venv"
	@echo "    source sphinx-venv/bin/activate"
	@echo "    pip install -r sphinx/requirements.txt"
	@echo
	@false

.DEFAULT_GOAL = all

else # HAVE_SPHINX

# Put it first so that "make" without argument is like "make help".
help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

.PHONY: help Makefile

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%: Makefile
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

endif # HAVE_SPHINX
