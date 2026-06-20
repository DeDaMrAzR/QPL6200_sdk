import sys

import click
from intelhex import IntelHex


@click.command()
@click.option('--overlap', default='error', help='Response to overlapping data in input',
              type=click.Choice(['error', 'keep_first', 'keep_last']))
@click.option('--ignore_start_execution_addr', is_flag=True, default=False, help="Ignore start execution address.")
@click.argument('output_hex', nargs=1)
@click.argument('input_hex', nargs=-1)
def main(overlap, ignore_start_execution_addr, output_hex, input_hex):
    if not input_hex:
        click.echo("Need at least one input hex file.", err=True)
        sys.exit(-1)

    if overlap == 'keep_first':
        overlap = 'ignore'
    elif overlap == 'keep_last':
        overlap = 'replace'

    output = IntelHex()
    if not ignore_start_execution_addr:
        output.start_addr = {'EIP': 0}

    for input_file in input_hex:
        input_data = IntelHex(input_file)
        if ignore_start_execution_addr and input_data.start_addr:
            # Remove the start execution address record
            input_data.start_addr = None
        output.merge(input_data, overlap=overlap)

    if not ignore_start_execution_addr:
        output.start_addr = {'EIP': 0}

    output.write_hex_file(output_hex)


if __name__ == '__main__':
    main()
