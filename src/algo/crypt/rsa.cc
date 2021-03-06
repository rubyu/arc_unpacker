#include "algo/crypt/rsa.h"
#include <openssl/bn.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include "err.h"

using namespace au;
using namespace au::algo::crypt;

struct Rsa::Priv final
{
    Priv(const RsaKey &key);
    ~Priv();

    RSA *key_impl;
};

Rsa::Priv::Priv(const RsaKey &key) : key_impl(RSA_new())
{
    BIGNUM *bn_modulus = BN_new();
    BIGNUM *bn_exponent = BN_new();
    BN_set_word(bn_exponent, key.exponent);
    BN_bin2bn(key.modulus.data(), key.modulus.size(), bn_modulus);

    key_impl->e = bn_exponent;
    key_impl->n = bn_modulus;
}

Rsa::Priv::~Priv()
{
    // BN_free(key_impl->e)?
    // BN_free(key_impl->n)?
    RSA_free(key_impl);
}

Rsa::Rsa(const RsaKey &key) : p(new Priv(key))
{
}

Rsa::~Rsa()
{
}

bstr Rsa::decrypt(const bstr &input) const
{
    const auto output_size = RSA_size(p->key_impl);
    auto output = std::make_unique<u8[]>(output_size);

    const auto result = RSA_public_decrypt(
        input.size(),
        input.get<const u8>(),
        output.get(),
        p->key_impl,
        RSA_PKCS1_PADDING);

    if (result == -1)
    {
        auto err = std::make_unique<char[]>(130);
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), err.get());
        throw err::CorruptDataError(std::string(err.get()));
    }

    return bstr(reinterpret_cast<char*>(output.get()), output_size);
}
