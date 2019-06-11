"""VNC Hook.

Opens a VNC connection with the Guest at the given event.

Configuration:

   {
     "start_vnc": "event_at_which_starting_vnc"
   }

"""

import subprocess

from see import Hook


class VNCHook(Hook):
    def __init__(self, parameters):
        super().__init__(parameters)

        if 'start_vnc' in self.configuration:
            self.context.subscribe_async(
                self.configuration['start_vnc'], self.vnc_handler)

    def vnc_handler(self, event):
        self.logger.info("Event %s: starting VNC connection.", event)

        command = ('virt-viewer', '--connect',
                   'qemu:///system', self.identifier)
        subprocess.call(command)