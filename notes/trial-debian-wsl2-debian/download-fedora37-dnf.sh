# cbuild --debug build-container fedora 37 x86_64-2022-12-22.0

# skip login local/target registry
#podman login

# pull base image
buildah from --isolation=chroot registry.fedoraproject.org/fedora:37
# run command
buildah run --isolation chroot fedora-working-container dnf upgrade -y --setopt=install_weak_deps=False --nogpgcheck fedora-release fedora-repos*
# error allocatestack.c:192: advise_stack_range: Assertion `freesize < size' failed. "dnf" -> update buildas 1.28 (bookworm)

buildah run --isolation chroot fedora-working-container dnf upgrade -y --setopt=install_weak_deps=False
