#include <complex.h>
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
                 KC_W,    KC_E,    KC_R,                       KC_U,    KC_I,    KC_O,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,     KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,     KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                 LT(_NAV, KC_TAB), LSFT_T(KC_SPC),     RSFT_T(KC_BSPC), LT(_SYM, KC_ENT)
    ),

    [_NAV] = LAYOUT(
                 KC_Q,    XXXXXXX, KC_T,                       KC_Y,    XXXXXXX, KC_P,
        KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, KC_ESC,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_DEL,
        QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, KC_CAPS,  KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_INS,
                                _______, _______,     _______, _______
    ),

    [_SYM] = LAYOUT(
                 KC_EXLM, KC_AT,   KC_HASH,                    KC_ASTR, KC_LPRN, KC_RPRN,
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        KC_GRV,  KC_BSLS, KC_MINS, KC_EQL,  KC_DLR,   KC_PERC, KC_LBRC, KC_RBRC, KC_QUOT, KC_TILD,
                                _______, _______,     _______, _______
    )
};
#include QMK_KEYBOARD_H

#define COMBO_ONLY_FROM_LAYER 0

#include "oneshot.h"
#include "swapper.h"

enum layers {
  DEF,
  SYM,
  NUM,
  NAV,
  FUN
};

enum keycodes {
    // Custom oneshot mod implementation with no timers.
    OS_SHFT = SAFE_RANGE,
    OS_CTRL,
    OS_ALT,
    OS_CMD,
    OS_SYS,
    OS_SYM,
    OS_CNCL,

    SW_WIN,
    SW_LANG,
    SW_TAB,  // TODO

    CM_UNDO,
    CM_CUT,
    CM_COPY,
    CM_PSTE,
    CM_SELA,
    CM_SAVE,
    CM_FIND,
    CM_NEW,
    CM_T,
    CM_W,

    CS_ARROW,
    CS_HXCMD,

    CC_CH,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     /*
      * ┌───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┐
      * │   │   │   │   │   │       │   │   │   │   │   │
      * ├───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┤
      * │ A │ S │ D │ F │ G │       │ H │   │   │   │   │
      * ├───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┤
      * │ Z │ X │ C │ V │ B │       │ N │ M │ , │ . │ / │
      * └───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┘
      *           ┌───┐                   ┌───┐
      *           │   ├───┐           ┌───┤   │
      *           └───┤SPC├───┐   ┌───┤BSP├───┘
      *               └───│LOW│   │RSE├───┘
      *                   └───┘   └───┘
      */


    [DEF] = LAYOUT(
                 KC_COMM, KC_DOT,  KC_P,                          KC_G,    KC_C,    KC_R,
        KC_A,    KC_O,    KC_E,    KC_U,    KC_I,        KC_D,    KC_H,    KC_T,    KC_N,    KC_S,
        KC_QUOT, KC_SEMI, KC_J,    KC_K,    KC_X,        KC_B,    KC_M,    KC_W,    KC_V,    KC_MINS,
                                   OS_SYM,  OS_SHFT,     KC_SPC,  MO_NAV
    ),

    [NUM] = LAYOUT(
                 KC_F11,  KC_F13,  KC_F14,                        KC_1,    KC_2,    KC_3,
        KC_F15,  KC_F16,  KC_F17,  KC_F18,  KC_19,       TO(FUN), KC_4,    KC_5,    KC_6,    KC_0,
        KC_F20,  KC_F21,  KC_F22,  KC_F23,  KC_F24,      CM_000,  KC_7,    KC_8,    KC_9,    KC_ENT,
                                   _______, _______,     _______, _______
    ),

    [SYM] = LAYOUT(
                 KC_QUES, KC_EXLM, KC_Y,                          KC_PIPE, KC_SLSH, KC_BSLS,
        KC_AMPR, KC_LBRC, KC_LCBR, KC_LPRN, KC_RCBR,     KC_EQL,  KC_ASTR, KC_RPRN, KC_PLUS, KC_RBRC,
        KC_GRV,  KC_HASH, KC_TILD, KC_DLR,  KC_Q,        KC_F,    KC_AT,   KC_PRCT, KC_CIRC, KC_L,
                                   _______, _______,     _______, TO(NUM)
    ),

    [NAV] = LAYOUT(
                 KC_F11,  KC_F13,  KC_F14,                        KC_PGUP, KC_UP,   KC_PGDN,
        CM_SELA, KC_F16,  KC_F17,  KC_F18,  KC_19,       KC_DEL,  KC_LEFT, KC_DOWN, KC_RGHT, KC_BSPC,
        CM_UNDO, CM_CUT,  CM_COPY, CM_PSTE, XXXXXXX,     TO(NAV), KC_HOME, KC_END,  KC_V,    KC_Z,
                                   TO(NUM), _______,     _______, _______
    ),

    [FUN] = LAYOUT( // TODO AU_VOL
                 KC_VOLU, XXXXXXX, XXXXXXX,                       KC_F1,   KC_F2,   KC_F3,
        CK_TOGG, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX,     KC_F10,  KC_F4,   KC_F5,   KC_F6,   KC_F11,
        XXXXXXX, KC_VOLD, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, KC_F7,   KC_F8,   KC_F9,   KC_F12,
                                   _______, _______,     _______, _______
    ),

};

const uint16_t PROGMEM cmb_o_e_u[] = {KC_O, KC_E, KC_U, COMBO_END};
const uint16_t PROGMEM cmb_a_o_e_u[] = {KC_A, KC_O, KC_E, KC_U, COMBO_END};

const uint16_t PROGMEM cmb_j_k[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM cmb_scln_j[] = {KC_SCLN, KC_J, COMBO_END};
const uint16_t PROGMEM cmb_scln_j_k[] = {KC_SCLN, KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM cmb_scln_j_u[] = {KC_SCLN, KC_J, KC_U, COMBO_END};
const uint16_t PROGMEM cmb_quot_scln_j_k[] = {KC_QUOT, KC_SCLN, KC_J, KC_K, COMBO_END};

const uint16_t PROGMEM cmb_comma_dot[] = {KC_COMMA, KC_DOT, COMBO_END};
const uint16_t PROGMEM cmb_comma_dot_p[] = {KC_COMMA, KC_DOT, KC_P, COMBO_END};
const uint16_t PROGMEM cmb_dot_p[] = {KC_DOT, KC_P, COMBO_END};

const uint16_t PROGMEM cmb_u_comma_dot[] = {KC_U, KC_COMMA, KC_DOT, COMBO_END};
const uint16_t PROGMEM cmb_o_e_i[] = {KC_O, KC_E, KC_I, COMBO_END};
const uint16_t PROGMEM cmb_a_o_e_i[] = {KC_A, KC_O, KC_E, KC_I, COMBO_END};
const uint16_t PROGMEM cmb_o_e_k[] = {KC_O, KC_E, KC_K, COMBO_END};
const uint16_t PROGMEM cmb_a_o_e_k[] = {KC_A, KC_O, KC_E, KC_K, COMBO_END};


const uint16_t PROGMEM cmb_h_t_n[] = {KC_H, KC_T, KC_N, COMBO_END};
const uint16_t PROGMEM cmb_h_t_n_s[] = {KC_H, KC_T, KC_N, KC_S, COMBO_END};

const uint16_t PROGMEM cmb_m_w[] = {KC_M, KC_W, COMBO_END};
const uint16_t PROGMEM cmb_w_v[] = {KC_W, KC_V, COMBO_END};
const uint16_t PROGMEM cmb_m_w_v[] = {KC_M, KC_W, KC_V, COMBO_END};
const uint16_t PROGMEM cmb_w_v_mins[] = {KC_W, KC_V, KC_MINS, COMBO_END};
const uint16_t PROGMEM cmb_m_w_v_mins[] = {KC_M, KC_W, KC_V, KC_MINS, COMBO_END};

const uint16_t PROGMEM cmb_g_c_r[] = {KC_G, KC_C, KC_R, COMBO_END};
const uint16_t PROGMEM cmb_g_c[] = {KC_G, KC_C, COMBO_END};
const uint16_t PROGMEM cmb_c_r[] = {KC_C, KC_R, COMBO_END};
const uint16_t PROGMEM cmb_h_c_r[] = {KC_H, KC_C, KC_R, COMBO_END};
const uint16_t PROGMEM cmb_d_t_n[] = {KC_D, KC_T, KC_N, COMBO_END};
const uint16_t PROGMEM cmb_d_t_n_s[] = {KC_D, KC_T, KC_N, KC_S, COMBO_END};
const uint16_t PROGMEM cmb_m_t_n[] = {KC_M, KC_T, KC_N, COMBO_END};
const uint16_t PROGMEM cmb_m_t_n_s[] = {KC_M, KC_T, KC_N, KC_S, COMBO_END};

const uint16_t PROGMEM cmb_the[] = {KC_E, KC_T, KC_H, COMBO_END};
const uint16_t PROGMEM cmb_ogf[] = {KC_O, KC_G, KC_F, COMBO_END};
const uint16_t PROGMEM cmb_ntg[] = {KC_N, KC_T, KC_G, COMBO_END};
const uint16_t PROGMEM cmb_ogcr[] = {KC_O, KC_G, KC_C, KC_R, COMBO_END};
const uint16_t PROGMEM cmb_omcr[] = {KC_O, KC_M, KC_C, KC_R, COMBO_END};
const uint16_t PROGMEM cmb_and[] = {KC_A, KC_N, KC_D, COMBO_END};
const uint16_t PROGMEM cmb_wh[] = {KC_W, KC_H, COMBO_END};
const uint16_t PROGMEM cmb_when[] = {KC_W, KC_H, KC_E, KC_N, COMBO_END};
const uint16_t PROGMEM cmb_wher[] = {KC_W, KC_H, KC_E, KC_R, COMBO_END};
const uint16_t PROGMEM cmb_tha[] = {KC_T, KC_H, KC_A, COMBO_END};
const uint16_t PROGMEM cmb_wha[] = {KC_W, KC_H, KC_A, COMBO_END};
const uint16_t PROGMEM cmb_becr[] = {KC_B, KC_E, KC_C, KC_R, COMBO_END};
const uint16_t PROGMEM cmb_aoegtr[] = {KC_A, KC_O, KC_E, KC_G, KC_T, KC_R, COMBO_END};
const uint16_t PROGMEM cmb_aoegcr[] = {KC_A, KC_O, KC_E, KC_G, KC_C, KC_R, COMBO_END};
const uint16_t PROGMEM cmb_son[] = {KC_S, KC_O, KC_N, COMBO_END};
const uint16_t PROGMEM cmb_owgr[] = {KC_O, KC_W, KC_G, KC_R, COMBO_END};
const uint16_t PROGMEM cmb_ogcn[] = {KC_O, KC_G, KC_C, KC_N, COMBO_END};
const uint16_t PROGMEM cmb_short[] = {KC_H, KC_R, KC_T, KC_O, KC_S, COMBO_END};
const uint16_t PROGMEM cmb_gcrp_dot[] = {KC_G, KC_C, KC_R, KC_P, KC_DOT, COMBO_END};
const uint16_t PROGMEM cmb_odcr[] = {KC_O, KC_D, KC_C, KC_R, COMBO_END};
const uint16_t PROGMEM cmb_ch[] = {KC_C, KC_H, COMBO_END};

combo_t key_combos[] = {
    COMBO(cmb_j_k, KC_ENT),
    COMBO(cmb_scln_j, KC_BSPC),
    COMBO(cmb_comma_dot, CS_ARROW),

    COMBO(cmb_dot_p, KC_Y),
    COMBO(cmb_comma_dot_p, CS_HXCMD),

    COMBO(cmb_scln_j_u, CS_QU),
    COMBO(cmb_quot_scln_j_k, KC_Q),

    COMBO(cmb_o_e_k, OS_SYS),
    COMBO(cmb_o_e_i, OS_ALT),
    COMBO(cmb_o_e_u, OS_CTRL),
    COMBO(cmb_a_o_e_u, KC_ESC),

    COMBO(cmb_g_c_r, KC_F),
    COMBO(cmb_h_t_n, KC_L),
    COMBO(cmb_h_t_n_s, CM_LL),
    COMBO(cmb_w_v_mins, KC_Z)

    COMBO(cmb_m_w, KC_TAB),
    COMBO(cmb_w_v, SW_WIN),
    COMBO(cmb_m_w_v, LALT(KC_TAB)),


    COMBO(cmb_ch, )
};

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case OS_CNCL:
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case TD_NAV:
    case MO_NAV:
    case TO(DEF):
    case TO(SYM):
    case TO(NAV):
    case MO(NAV):
    case TO(NUM):
    case TO(FUN):
    case KC_LSFT:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_CMD:
    case OS_SYS:
    case OS_SYM:
        return true;
    default:
        return false;
    }
}

bool sw_win_active = false;
bool sw_lang_active = false;
bool sw_tab_active = false;

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;
oneshot_state os_sys_state = os_up_unqueued;
oneshot_state os_sym_state = os_up_unqueued;

int with_ctrl(int keycode) {
    return C(keycode);
}
int with_cmd(int keycode) {
    return G(keycode);
}
int (*sys_mod)(int) = with_ctrl;
int sys_mod_kc = KC_LCTL;
bool process_detected_host_os_user(os_variant_t detected_os) {
    switch (detected_os) {
        case OS_MACOS:
        case OS_IOS:
            sys_mod = with_cmd;
            sys_mod_kc = KC_LGUI;
            break;
        default:
            break;
    }
    return true;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    update_swapper(
        &sw_win_active, KC_LGUI, KC_TAB, SW_WIN,
        keycode, record
    );
    update_swapper(
        &sw_lang_active, KC_LCTL, KC_SPC, SW_LANG,
        keycode, record
    );
    update_swapper(
        &sw_tab_active, KC_LCTL, KC_TAB, SW_TAB,
        keycode, record
    );

    update_oneshot(
        &os_shft_state, KC_LSFT, OS_SHFT,
        keycode, record
    );
    update_oneshot(
        &os_ctrl_state, KC_LCTL, OS_CTRL,
        keycode, record
    );
    update_oneshot(
        &os_alt_state, KC_LALT, OS_ALT,
        keycode, record
    );
    update_oneshot(
        &os_cmd_state, KC_LCMD, OS_CMD,
        keycode, record
    );
    update_oneshot(
        &os_cmd_state, sys_mod_kc, OS_SYS,
        keycode, record
    );
    update_oneshot_layer(
        &os_sym_state, SYM, OS_SYM,
        keycode, record
    );

    void (*reg_unreg)(uint16_t) = register_code16;
    if (!record->event.pressed) {
        reg_unreg = unregister_code16;
    }
    switch (keycode) {
    case MO_NAV:
        if(record->event.pressed) {
            layer_on(NAV);
        } else {
            sw_win_active = false;
            sw_lang_active = false;
            os_shft_state = os_up_unqueued;
            os_ctrl_state = os_up_unqueued;
            os_alt_state = os_up_unqueued;
            os_cmd_state = os_up_unqueued;
            os_sys_state = os_up_unqueued;
            os_sym_state = os_up_unqueued;
            reset_tap_dance(state);
            clear_keyboard();
            layer_clear();
        }
        break;
    case CS_ARROW:
        SEND_STRING("->");
        break;
    case CS_HXCMD:
        tap_code(KC_ESC);
        tap_code(KC_COLON);
        break;
    case CM_UNDO:
        reg_unreg(sys_mod(KC_Z));
        break;
    case CM_CUT:
        reg_unreg(sys_mod(KC_X));
        break;
    case CM_COPY:
        reg_unreg(sys_mod(KC_C));
        break;
    case CM_PSTE:
        reg_unreg(sys_mod(KC_V));
        break;
    case CM_SELA:
        reg_unreg(sys_mod(KC_A));
        break;
    case CM_SAVE:
        reg_unreg(sys_mod(KC_S));
        break;
    case CM_FIND:
        reg_unreg(sys_mod(KC_F));
        break;
    case CM_NEW:
        reg_unreg(sys_mod(KC_N));
        break;
    case CM_T:
        reg_unreg(sys_mod(KC_T));
        break;
    case CM_W:
        reg_unreg(sys_mod(KC_W));
        break;
    case CC_THE:
        SEND_STRING("the");
        break;
    case CC_:
        SEND_STRING("");
        break;
    case CC_:
        SEND_STRING("");
        break;
    case CC_:
        SEND_STRING("");
        break;
    }

    return true;
}

void cc(*char str) {
    switch(str[0]) {
    case '-':
        send_string("=");
        break;
    default:
        send_string()
    }
}
