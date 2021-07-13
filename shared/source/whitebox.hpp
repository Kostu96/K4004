// TODO: Move to shared location
#pragma once

template <typename T>
struct WhiteBox;

#ifdef K4004_TESTS
#define MOCKABLE_VIRTUAL virtual
#define ALLOW_WHITEBOX(BASE) friend struct WhiteBox<BASE>
#else
#define MOCKABLE_VIRTUAL
#define ALLOW_WHITEBOX(BASE)
#endif
