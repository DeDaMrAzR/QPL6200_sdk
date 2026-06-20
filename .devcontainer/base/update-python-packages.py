#!/usr/bin/python

import os
import sys
import subprocess
import json
import click
from west.manifest import Manifest
import logging

logger = logging.getLogger(__name__)

PIPX_INSTALL_COMMAND = 'sudo -E pipx install --editable <path> --force'
PIP_INSTALL_COMMAND = 'pip install -e <path>'

def west_get_python_projects():
    west_packages = {}
    manifest = Manifest.from_file()
    manifestproject, *projects = manifest.get_projects(None)
    for project in projects:
        if os.path.exists(os.path.join(project.abspath, "pyproject.toml")):
            west_packages[project.name] = project.abspath
            logger.debug(f"Python project found: {project.name} {project.abspath}")
    return west_packages


def list_pip_packages():
    '''
    Lets create a list with the installed pip packages
    and the command to override if required.
    '''
    pkg_list = []
    # List without editable, as editable package does not gave the name
    output = subprocess.check_output(['pip', 'freeze', '--exclude-editable'])
    pkgs = output.decode('utf-8').split()
    # Returns all editable packages with name !
    output = subprocess.check_output(['pip', 'list', '-e', '--format=freeze'])
    pkgs_editable = output.decode('utf-8').split()
    for pkg in pkgs + pkgs_editable:
        pkg_name = pkg.split('==')[0]
        pkg_list.append([pkg_name, PIP_INSTALL_COMMAND])
    return pkg_list

def list_pipx_packages():
    '''
    Lets create a list with the installed pipx packages, their injected packages, dependencies
    and the command to override if required.
    '''
    pkg_list = []
    pkg_injected_list = []
    pipx_packages = subprocess.check_output(['sudo', '-E', 'pipx', 'list', '--include-injected', '--json'])
    y = json.loads(pipx_packages.decode('utf-8'))
    for pkg_name, metadata in y['venvs'].items():
        pkg_list.append([pkg_name, PIPX_INSTALL_COMMAND])
        for injected_pkg_name in metadata['metadata']['injected_packages'].keys():
            pkg_injected_list.append([injected_pkg_name, f'sudo -E pipx inject {pkg_name} --editable <path>'])
            for dependency_injected_pkg_name in metadata['metadata']['injected_packages'][injected_pkg_name]['apps_of_dependencies']:
                pkg_injected_list.append([dependency_injected_pkg_name, f'sudo -E pipx inject {pkg_name} --editable <path>'])
    return pkg_list, pkg_injected_list

def update_or_install_package(west_pkg_name, west_pkg_path, command, force=False):
    cmd = command.replace('<path>', west_pkg_path)
    if force:
        logger.warning(f"Install or updating package {west_pkg_name} - {cmd}")
        subprocess.run(cmd.split())
    else:
        while True:
            user_input = input(f"Do you want execute {cmd}? (yes/no): ")
            if user_input.lower() in ["yes", "y"]:
                logger.warning(f"Install or updating package {west_pkg_name} - {cmd}")
                subprocess.run(cmd.split())
                break
            elif user_input.lower() in ["no", "n"]:
                logger.warning(f"Skipping package {west_pkg_name} - {cmd}")
                break
            else:
                print("Invalid input. Please enter yes/no.")


@click.command()
@click.option("--force", help="Do not ask for Y/N on the command line",is_flag=True, default=False)
def update_pkgs(force) -> None:
    logger.info("Running update-python-packages ...")
    west_python_packages = west_get_python_projects()
    if not west_python_packages:
        logger.warning(f"No python package found in the manifest, skipping ...")
        return
    pip_packages = list_pip_packages()
    pipx_packages, pipx_injected_packages = list_pipx_packages()

    west_packages_to_process = list(west_python_packages.keys())
    # Update injected, pipx and pip packages
    all_installed_packages = pip_packages + pipx_packages + pipx_injected_packages
    for west_pkg_name, west_pkg_path in west_python_packages.items():
        for pkg_with_cmd in all_installed_packages:
            if west_pkg_name == pkg_with_cmd[0]:
                update_or_install_package(west_pkg_name, west_pkg_path, pkg_with_cmd[1], force=force)
                try:
                    west_packages_to_process.remove(west_pkg_name)
                except ValueError:
                    pass

    for package in west_packages_to_process:
        logger.warning(f"{package} not found in devcontainer ! Skip installation")

if __name__ == "__main__":
    update_pkgs()
