ROOTFS_POSTPROCESS_COMMAND:append = " enable_system_fips;"

enable_system_fips() {
    install -d ${IMAGE_ROOTFS}${sysconfdir}
    touch ${IMAGE_ROOTFS}${sysconfdir}/system-fips
}
