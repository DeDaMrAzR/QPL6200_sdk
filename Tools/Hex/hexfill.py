import click
from intelhex import IntelHex


class BasedIntParamType(click.ParamType):
    name = 'integer'

    def convert(self, value, param, ctx):
        if isinstance(value, int):
            return value
        try:
            return int(value, 0)
        except ValueError:
            self.fail('%s is not a valid integer' % value, param, ctx)


BASED_INT = BasedIntParamType()


@click.command()
@click.argument('in_file')
@click.argument('out_file')
@click.option('--value', '-v', default=0, type=BASED_INT, help="Byte value used to fill the address range.")
@click.option('--start', '-s', default=None, type=BASED_INT, help="Start address (inclusive).")
@click.option('--endex', '-e', default=None, type=BASED_INT, help="End address (exclusive).")
@click.option('--ignore_start_execution_addr', is_flag=True, default=False, help="Ignore start execution address.")
def main(in_file, out_file, value, start, endex, ignore_start_execution_addr):
    data = IntelHex(in_file)

    if not start:
        start = data.minaddr()
    if not endex:
        endex = data.maxaddr() + 1

    fill_data = IntelHex()
    fill_data.puts(start, chr(value) * (endex - start))

    data.merge(fill_data, 'ignore')

    if not ignore_start_execution_addr:
        data.start_addr = {'EIP': start}

    data.write_hex_file(out_file)


if __name__ == '__main__':
    main()
