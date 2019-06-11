#!/usr/bin/env python3

import time
import argparse

from see import Environment
from see.context import QEMUContextFactory

TIMEOUT = 60
RUNTIME = 60


def main():
    import sys
    print(sys.argv)
    arguments = parse_arguments()

    context_factory = QEMUContextFactory(arguments.context)

    with Environment(context_factory, arguments.hooks) as environment:
        protocol(environment.context, arguments.sample, arguments.command)


def protocol(context, sample_path, execution_command):
    context.poweron()

    wait_for_ip_address(context, TIMEOUT)

    context.trigger('run_sample', sample=sample_path,
                    command=execution_command)

    time.sleep(RUNTIME)

    context.pause()
    context.trigger('snapshots_capture')
    context.resume()

    context.shutdown(timeout=TIMEOUT)

    context.trigger('start_analysis')
    context.trigger('wait_analysis')


def wait_for_ip_address(context, timeout):
    timestamp = time.time()

    while time.time() - timestamp < timeout:
        if context.ip4_address is not None:
            context.trigger('ip_address', address=context.ip4_address)
            return

        time.sleep(1)

    raise TimeoutError("Waiting for IP address")


def parse_arguments():
    parser = argparse.ArgumentParser(
        description='Execute a sample within a Sandbox.')

    parser.add_argument('context', help='path to Context JSON configuration')
    parser.add_argument('sample', help='path to Sample to execute')
    parser.add_argument('-k', '--hooks', default={},
                        help='path to Hooks JSON configuration')
    parser.add_argument('-c', '--command', default='start {sample}',
                        help="""command used to start the sample.
                        The string {sample} will be expanded to the actual file name within the guest.
                        Example: 'notepad.exe {sample}'""")
    return parser.parse_args()


if __name__ == '__main__':
    main()
