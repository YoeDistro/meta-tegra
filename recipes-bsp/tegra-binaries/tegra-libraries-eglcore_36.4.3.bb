L4T_DEB_COPYRIGHT_MD5 = "d3617777039321a257aef01439341b02"
DEPENDS = "tegra-libraries-core"

L4T_DEB_TRANSLATED_BPN = "nvidia-l4t-3d-core"

require tegra-debian-libraries-common.inc

MAINSUM = "ab19b524f942b35d21cc7893cf5b8a5d0a4007614e88c27dd85e2668a537cc5c"

TEGRA_LIBRARIES_TO_INSTALL = "\
    tegra-egl/libEGL_nvidia.so.0 \
    nvidia/libnvidia-eglcore.so.${L4T_LIB_VERSION} \
    nvidia/libnvidia-glsi.so.${L4T_LIB_VERSION} \
    nvidia/libnvidia-glvkspirv.so.${L4T_LIB_VERSION} \
    nvidia/libnvidia-rtcore.so.${L4T_LIB_VERSION} \
"

do_install() {
    install_libraries
    install -d ${D}${datadir}/glvnd/egl_vendor.d
    install -m644 ${S}/usr/lib/aarch64-linux-gnu/tegra-egl/nvidia.json ${D}${datadir}/glvnd/egl_vendor.d/10-nvidia.json
}

pkg_postinst:${PN}() {
    # argus and scf libraries hard-coded to use this path
    install -d $D/usr/lib/aarch64-linux-gnu/tegra-egl
    ln -f $D${libdir}/libEGL_nvidia.so.0 $D/usr/lib/aarch64-linux-gnu/tegra-egl/
}

FILES:${PN} += "${datadir}/glvnd/egl_vendor.d"
