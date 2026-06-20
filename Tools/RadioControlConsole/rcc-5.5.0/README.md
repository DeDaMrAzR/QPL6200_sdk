# Radio Control Console

## Description

This package is a Python implementation of the Radio Control Console package v4.0.0.
The 4.0.0 package was written in C# and released for Windows as an executable with an `*.msi` installer.


## RTFM

Read the application's [documentation](https://qorvo.gitlab.io/tools/packages/rcc/index.html).

The [Getting started with Radio Control Console Python package](getting_started_for_customers.md)
page contains a quick guide that can be shared with customers and SDK packages.


## How to install

The project is distributed in two manners: through binary packages and through an internal Qorvo package registry.

Installation through binary files is done with the following command: `python3 -m pip install rcc-0.0.1rc1.tar.gz`.
The main drawback, however, is that updates have to be made manually.

It is **recommended** that the package is installed through the package registry for the main reason of easily enabling updates.
The package registry is located [here](https://gitlab.com/qorvo/wcon/qmodules/registry/-/packages).

To install via commandline, execute:

```
python3 -m pip install rcc --index-url https://gitlab.com/api/v4/projects/46858891/packages/pypi/simple
```

To add the package to a `pip_requirements.txt` file, use:

```
# pip_requirements.txt
--extra-index-url=https://gitlab.com/api/v4/projects/46858891/packages/pypi/simple
rcc
```

**Note:** to use the internal repository

## How to use

With RCC installed, it should be available as a Python module with an entrypoint, accessible through
```
python3 -m rcc
```
The project requires at least Python 3.10.


## How to develop

Since this project is distributed as a package, is recommended to create a virtual python environment and develop the package with it.
The steps for that are:
```
python3 -m venv .venv
./.venv/Scripts/<SCRIPT>
python3 -m pip install -e .
```

Where `<SCRIPT>` is replaced by the appropriate system's script.

It is enforced that changes are added to a new branch. The branch naming scheme `mr/<NAME>` is suggested.

Commits in this repository are enforced to follow the conventional commits scheme. In conventional commits, a pre-defined format is mandatory, along with a JIRA ticket.
An example commit message is:
```
feat(rcc): Add something to another thing

In this commit we added a feature called something to
another block called another thing.
This was done because we need to have something done.
And that's how we move forward.

Refs: APPHW-2487
```

By default, unhandled exceptions are suppressed; only their error message is shown and the application exits.
In some scenarios is interesting to be able to read the complete traceback of the failure that caused a fatal error.
For this cases, when the environment variable `RCC_DEBUG` is set to `true` (all lower-case) the application will "re-raise" the exception,
causing the complete stack trace to be shown to the user.
This feature is UNDOCUMENTED to external parties.


### VSCode Workspace

A pre-configured VSCode workspace is available in `.vscode/rcc.code-workspace`.
The workspace has the appropriate styling and linting configuration, as well as the recommended extensions for development.


## How to build

Ensure that your Python version is >= 3.10, and that Hatchling and Python's native build system are installed.
```
python --version
python -m pip install build
python -m pip install hatchling
```
Then, from this repository's root, run:
```
python -m build
```
This should create a dist folder with pip installable artifacts.



## How to release

All releases shall be derived from a branch following the naming convention `release/<X>.<Y>`, where `X` and `Y` are respectively the major and minor versions of the release.
The release pipeline is triggered when a new `git tag` is pushed to the server. Additionally, the pipepline checks if the tag naming matches the name declared in the `qorvo/rcc/__init__.py` file.

Therefore, to make a release the following steps shall be followed:
	- Create (or checkout) the `release/<X>.<Y>.x` branch
	- Modify the version in `qorvo/rcc/__init__.py`, optionally marking the state as a release candidate (`rc`); make a commit
	- Add additional changes, via new commits or cherry-picking
	- Create a tag with the version, e.g. `0.1.2`; if applicable, qualify with `rc1`, `rc2`, `rc3`, etc.
	- Go to the web browser and check the pipeline

If the pipeline is successful, the release has been completed successfuly. Check the package in the project's package registry.
