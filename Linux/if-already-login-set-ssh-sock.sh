if [ -z "$SSH_AUTH_SOCK" ]; then
  if [ -n $(who | cut -d' ' -f1 | sort | uniq) ]; then
     export SSH_AUTH_SOCK="/run/user/1000/keyring/ssh"
  fi
fi
