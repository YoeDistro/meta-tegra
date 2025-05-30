L4T_DEB_COPYRIGHT_MD5 = "770b0fc2a5cffa1d2b7eda7393e6b012"
DEPENDS = "tegra-libraries-core tegra-libraries-cuda tegra-libraries-multimedia tegra-libraries-multimedia-utils"

L4T_DEB_TRANSLATED_BPN = "nvidia-l4t-multimedia"

require tegra-debian-libraries-common.inc

MAINSUM = "a3067000eb63ebe954684367464e7f1de213abb0c78ec3400a59ceb9acdf69bb"

inherit features_check

REQUIRED_DISTRO_FEATURES = "opengl"

TEGRA_LIBRARIES_TO_INSTALL = "\
    nvidia/libtegrav4l2.so \
    nvidia/libnvcuvidv4l2.so \
"

TEGRA_PLUGINS = "\
    libv4l2_nvvideocodec.so \
    libv4l2_nvcuvidvideocodec.so \
"

do_install() {
    install_libraries
    install -d ${D}${libdir}/libv4l/plugins
    for f in ${TEGRA_PLUGINS}; do
        install -m 0644 ${S}/usr/lib/aarch64-linux-gnu/nvidia/$f ${D}${libdir}/libv4l/plugins/
    done
}

FILES_SOLIBSDEV = ""
SOLIBS = ".so*"
FILES:${PN} += "${libdir}/libv4l"
