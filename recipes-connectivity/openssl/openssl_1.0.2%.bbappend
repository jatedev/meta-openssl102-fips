FIPSINC = ""
FIPSINC:class-target = "${@'' if d.getVar('OPENSSL_FIPS_ENABLED', True) != '1' else 'openssl_fips.inc'}"

require ${FIPSINC}
