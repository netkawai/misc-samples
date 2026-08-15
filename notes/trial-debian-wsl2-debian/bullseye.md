This is note to install podman4 and buildah 1.28 into bullseye (Debian11)
As usual, the release cycle of Debian is 2-3 years to compare with 0.5 years in Fedora.

Now, this is so norm, Debian is always somewhat old or behind, if you want to run the latest Fedora repository, you cannot. As of February 2023, Debian is still 11 (bullseye August 2021), Fedora released 37 which include libc6-2.36 against 2.34 in Debian 11
Nowadays, especially podman and buildah tight with system, they are dependency breaker...

luckily, now bookworm, now alway freezing phase to release.
I just downloaded libc6-packages in this place.
http://http.us.debian.org/debian/pool/main/g/glibc/libc6_2.36-8_amd64.deb

Now, Debian team has only http in North America.

Then install dpkg --install --force-depends libc6_2.36-8_amd64.deb

Usually upgrade (not downgrade) will not cause issue, since most cases they will add (not remove)

then I download buildah itself below. (because of anti-virus, I cannot use below link directly, so I need to open the folder fist golang-github-container-buildah, then click deb file)
http://http.us.debian.org/debian/pool/main/g/golang-github-containers-buildah/buildah_1.28.2+ds1-1+b2_amd64.deb

Before that, I followed below link. but I think, I can install through deb though
https://computingforgeeks.com/how-to-install-podman-on-debian-linux/
but not slirp4netns

Since now, Debian is only purely community based (partly by Canonical but not fully and of course NOT RedHat). No communities are catching up RedHat man-power...

