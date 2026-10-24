#include QMK_KEYBOARD_H

#include "swapper.h"

// Port of the jacob_2 ZMK Totem layout, cut down to 30 keys. The top-row pinky
// keys, the top-row inner index keys and the outer thumb keys are unused.

enum layers {
    DEF,
    GAM,
    MAC, // TODO: trilayer for mac layers (not reachable yet, same as the Totem)
    NUM,
    NAV,
    FUN,
    CUS,
    SYM,
};

enum keycodes {
    // &exit: return to DEF, then press the wrapped key.
    EX_COMM = SAFE_RANGE,
    EX_DOT,
    EX_SPC,
    EX_ENT,
    EX_BSPC,
    EX_ESC,

    GO_DEF,  // &def: return to DEF and cancel sticky keys
    ESC_NAV, // &esc_nav: go to NAV, cancel sticky keys, tap Esc
    SYM_DEF, // &sym: return to DEF, then sticky SYM
    WIN_NXT, // &win_nxt: hold Alt and tap Tab until another key is hit
};

#define SK_ALT OSM(MOD_LALT)
#define SK_SFT OSM(MOD_LSFT)
#define SK_CTL OSM(MOD_LCTL)
#define SK_GUI OSM(MOD_LGUI)

// The 30 keys in use; the other 6 are always KC_NO.
// clang-format off
#define LAYOUT_30(                                                  \
         L01, L02, L03,                     R01, R02, R03,           \
    L10, L11, L12, L13, L14,      R10, R11, R12, R13, R14,           \
    L20, L21, L22, L23, L24,      R20, R21, R22, R23, R24,           \
                   L30, L31,      R30, R31                           \
) LAYOUT_split_3x5_3(                                               \
    XXXXXXX, L01, L02, L03, XXXXXXX,    XXXXXXX, R01, R02, R03, XXXXXXX, \
    L10,     L11, L12, L13, L14,        R10,     R11, R12, R13, R14,     \
    L20,     L21, L22, L23, L24,        R20,     R21, R22, R23, R24,     \
         XXXXXXX, L30, L31,             R30, R31, XXXXXXX                \
)
// clang-format on

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     *     ┌───┬───┬───┐               ┌───┬───┬───┐
     *     │ , │ . │ P │               │ G │ C │ R │
     * ┌───┼───┼───┼───┼───┐       ┌───┼───┼───┼───┼───┐
     * │ A │ O │ E │ U │ I │       │ D │ H │ T │ N │ S │
     * ├───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┤
     * │ ' │ Q │ J │ K │ X │       │ B │ M │ W │ V │ Z │
     * └───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┘
     *               ┌───┐           ┌───┐
     *               │SYM├───┐   ┌───┤NAV│
     *               └───┤SFT│   │SPC├───┘
     *                   └───┘   └───┘
     *
     * Y, F, L, ; and : are on NAV. Ctrl, Alt and GUI are combos.
     */
    [DEF] = LAYOUT_30(
                 KC_COMM, KC_DOT,  KC_P,                 KC_G,    KC_C,    KC_R,
        KC_A,    KC_O,    KC_E,    KC_U,    KC_I,        KC_D,    KC_H,    KC_T,    KC_N,    KC_S,
        KC_QUOT, KC_Q,    KC_J,    KC_K,    KC_X,        KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,
                                   SYM_DEF, SK_SFT,      EX_SPC,  MO(NAV)
    ),

    // Games need a held (not sticky) Ctrl, so it takes the ' key.
    [GAM] = LAYOUT_30(
                 KC_COMM, KC_DOT,  KC_P,                 KC_G,    KC_C,    KC_R,
        KC_A,    KC_O,    KC_E,    KC_U,    KC_I,        KC_D,    KC_H,    KC_T,    KC_N,    KC_S,
        KC_LCTL, KC_Q,    KC_J,    KC_K,    KC_X,        KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,
                                   KC_LSFT, KC_SPC,      EX_ENT,  SK_ALT
    ),

    [MAC] = LAYOUT_30(
                 KC_COMM, KC_DOT,  KC_P,                 KC_G,    KC_C,    KC_R,
        KC_A,    KC_O,    KC_E,    KC_U,    KC_I,        KC_D,    KC_H,    KC_T,    KC_N,    KC_S,
        KC_QUOT, KC_Q,    KC_J,    KC_K,    KC_X,        KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,
                                   _______, _______,     _______, _______
    ),

    [NUM] = LAYOUT_30(
                 KC_COMM, KC_DOT,  KC_SPC,               KC_1,    KC_2,    KC_3,
        C(KC_A), KC_UNDS, OSL(SYM),KC_BSPC, KC_PLUS,     TO(FUN), KC_4,    KC_5,    KC_6,    KC_0,
        C(KC_Z), C(KC_X), C(KC_C), C(KC_V), XXXXXXX,     XXXXXXX, KC_7,    KC_8,    KC_9,    KC_ENT,
                                   _______, _______,     _______, _______
    ),

    // Held from the right thumb, the left-hand letters are typed cross-hand.
    [NAV] = LAYOUT_30(
                 EX_COMM, EX_DOT,  KC_SPC,               KC_PGUP, KC_UP,   KC_PGDN,
        C(KC_A), C(S(KC_TAB)), C(KC_TAB), WIN_NXT, S(KC_TAB), KC_DEL, KC_LEFT, KC_DOWN, KC_RGHT, KC_BSPC,
        KC_SCLN, KC_COLN, KC_L,    KC_F,    KC_Y,        KC_APP,  KC_HOME, KC_END,  KC_V,    KC_ENT,
                                   _______, _______,     _______, _______
    ),

    // The Totem's Bluetooth / output keys are replaced by the Dasbob's audio toggles.
    [FUN] = LAYOUT_30(
                 KC_VOLD, KC_MUTE, KC_VOLU,              KC_F1,   KC_F2,   KC_F3,
        CK_TOGG, KC_MPRV, KC_MPLY, KC_MNXT, KC_BRIU,     KC_F10,  KC_F4,   KC_F5,   KC_F6,   KC_F11,
        AU_TOGG, KC_MRWD, XXXXXXX, KC_MFFD, KC_BRID,     KC_INS,  KC_F7,   KC_F8,   KC_F9,   KC_F12,
                                   _______, _______,     _______, _______
    ),

    [CUS] = LAYOUT_30(
                   C(KC_F21), C(KC_F22), C(KC_F13),                  KC_F21, KC_F22, KC_F13,
        S(KC_F14), C(KC_F14), C(KC_F15), C(KC_F16), C(KC_F20),     A(KC_F14), KC_F14, KC_F15, KC_F16, KC_F20,
        S(KC_F17), C(KC_F17), C(KC_F18), C(KC_F19), C(KC_F24),     A(KC_F17), KC_F17, KC_F18, KC_F19, KC_F24,
                                         S(KC_F21), C(KC_F23),     A(KC_F21), KC_F23
    ),

    [SYM] = LAYOUT_30(
                 KC_QUES, KC_EXLM, KC_PERC,              KC_PIPE, KC_SLSH, KC_BSLS,
        KC_AMPR, KC_LBRC, KC_LCBR, KC_LPRN, KC_RCBR,     KC_EQL,  KC_ASTR, KC_RPRN, KC_PLUS, KC_RBRC,
        KC_GRV,  KC_HASH, KC_TILD, KC_DLR,  KC_SCLN,     XXXXXXX, KC_AT,   KC_MINS, KC_UNDS, KC_CIRC,
                                   _______, _______,     _______, _______
    ),
};

// Combos are positional (COMBO_ONLY_FROM_LAYER 0), so they fire on every layer
// like the Totem's ZMK combos. Keys are named by their DEF-layer keycodes.
const uint16_t PROGMEM cmb_o_e_u[]     = {KC_O, KC_E, KC_U, COMBO_END};
const uint16_t PROGMEM cmb_o_e_i[]     = {KC_O, KC_E, KC_I, COMBO_END};
const uint16_t PROGMEM cmb_o_e_k[]     = {KC_O, KC_E, KC_K, COMBO_END};
const uint16_t PROGMEM cmb_o_e_x[]     = {KC_O, KC_E, KC_X, COMBO_END};
const uint16_t PROGMEM cmb_a_o_e_u[]   = {KC_A, KC_O, KC_E, KC_U, COMBO_END};
const uint16_t PROGMEM cmb_q_j[]       = {KC_Q, KC_J, COMBO_END};
const uint16_t PROGMEM cmb_j_k[]       = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM cmb_q_j_k[]     = {KC_Q, KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM cmb_m_w[]       = {KC_M, KC_W, COMBO_END};
const uint16_t PROGMEM cmb_w_v[]       = {KC_W, KC_V, COMBO_END};
const uint16_t PROGMEM cmb_h_t_n[]     = {KC_H, KC_T, KC_N, COMBO_END};
const uint16_t PROGMEM cmb_t_n_s[]     = {KC_T, KC_N, KC_S, COMBO_END};
const uint16_t PROGMEM cmb_h_t_n_s[]   = {KC_H, KC_T, KC_N, KC_S, COMBO_END};
const uint16_t PROGMEM cmb_m_w_v[]     = {KC_M, KC_W, KC_V, COMBO_END};
const uint16_t PROGMEM cmb_quot_q_j_k[] = {KC_QUOT, KC_Q, KC_J, KC_K, COMBO_END};

combo_t key_combos[] = {
    COMBO(cmb_o_e_u, GO_DEF),
    COMBO(cmb_o_e_i, SK_ALT),
    COMBO(cmb_o_e_k, SK_CTL),
    COMBO(cmb_o_e_x, SK_GUI),
    COMBO(cmb_a_o_e_u, EX_ESC),
    COMBO(cmb_q_j, EX_BSPC),
    COMBO(cmb_j_k, EX_ENT),
    COMBO(cmb_q_j_k, CW_TOGG),
    COMBO(cmb_m_w, KC_TAB),
    COMBO(cmb_w_v, A(KC_TAB)),
    COMBO(cmb_h_t_n, TO(NAV)),
    COMBO(cmb_t_n_s, ESC_NAV),
    COMBO(cmb_h_t_n_s, TO(NUM)),
    COMBO(cmb_m_w_v, OSL(CUS)),
    COMBO(cmb_quot_q_j_k, TO(GAM)),
};

static uint16_t exit_keycode(uint16_t keycode) {
    switch (keycode) {
    case EX_COMM: return KC_COMM;
    case EX_DOT:  return KC_DOT;
    case EX_SPC:  return KC_SPC;
    case EX_ENT:  return KC_ENT;
    case EX_BSPC: return KC_BSPC;
    case EX_ESC:  return KC_ESC;
    default:      return KC_NO;
    }
}

// Equivalent of the Totem's K_CANCEL tap, which releases pending sticky keys.
static void cancel_sticky(void) {
    clear_oneshot_mods();
    reset_oneshot_layer();
    caps_word_off();
}

// Match ZMK's caps word: continue on letters, digits, underscore, backspace
// and delete; anything else (including minus) ends it.
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
    case KC_A ... KC_Z:
        add_weak_mods(MOD_BIT(KC_LSFT));
        return true;
    case KC_1 ... KC_0:
    case KC_UNDS:
    case KC_BSPC:
    case EX_BSPC:
    case KC_DEL:
        return true;
    default:
        return false;
    }
}

bool win_nxt_active = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Shift+Tab steps back through the switcher without releasing Alt.
    if (keycode != S(KC_TAB)) {
        update_swapper(
            &win_nxt_active, KC_LALT, KC_TAB, WIN_NXT,
            keycode, record
        );
    }

    uint16_t exit_kc = exit_keycode(keycode);
    if (exit_kc != KC_NO) {
        if (record->event.pressed) {
            layer_move(DEF);
            reset_oneshot_layer();
            register_code16(exit_kc);
        } else {
            unregister_code16(exit_kc);
        }
        return false;
    }

    switch (keycode) {
    case GO_DEF:
        if (record->event.pressed) {
            layer_move(DEF);
            cancel_sticky();
        }
        return false;
    case ESC_NAV:
        if (record->event.pressed) {
            layer_move(NAV);
            cancel_sticky();
            tap_code(KC_ESC);
        }
        return false;
    case SYM_DEF:
        if (record->event.pressed) {
            layer_move(DEF);
            // Tap (not hold) of a one-shot layer, like the ZMK macro.
            set_oneshot_layer(SYM, ONESHOT_START);
            clear_oneshot_layer_state(ONESHOT_PRESSED);
        }
        return false;
    case WIN_NXT:
        return false;
    }

    return true;
}
