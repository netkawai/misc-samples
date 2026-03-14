Somehow, when I enabled openssh-agent service in .basshrc
```bash
# Ubuntu for ssh-agent
if [ -z "$SSH_AUTH_SOCK" ] ; then
   systemctl --user start ssh-agent

   if [ $(ps ax | grep [s]sh-agent | wc -l) -gt 0 ] ; then
      export SSH_AUTH_SOCK=${XDG_RUNTIME_DIR}/openssh_agent
      ssh-add
    else
      echo "No ssh-agent !!!"
    fi
fi
```
I could not find a proper way to disable autostart gnome-keyring.
I noted down here. How I figured out?
I checked systemd file in the path.
```
 cat /lib/systemd/user/gnome-keyring-ssh.service
```
 Then, I saw this file is checking this way.
```
 ! grep -s -q X-GNOME-Autostart-enabled=false ~/.config/autostart/gnome-keyring-ssh.desktop /etc/xdg/autostart/gnome-keyring-ssh.desktop
```
this indicates that if that autostart file in user home directly, has "X-GNOME-Autostart-enabled=false" not restart, then they will not kick.

I am not sure why, I could figure out a couple of months ago.
