git config --global credential.helper '!f() { echo "username=netkawai"; echo "password=${GITHUB_TOKEN}"; }; f'


