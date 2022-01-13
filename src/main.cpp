#include <BetterGD.hpp>

using namespace bgd;

LILAC_API bool LILAC_CALL lilac_load(Mod* mod) {
    BetterGD::get()->init(mod);

    BetterGD::mod()->log() << "Hi from BetterGD" << lilac::endl;

    return true;
}

LILAC_API void LILAC_CALL lilac_unload() {

}

BetterGD* BetterGD::get() {
    static auto g_mod = new BetterGD;
    return g_mod;
}

Mod* BetterGD::mod() {
    return BetterGD::get()->m_mod;
}
