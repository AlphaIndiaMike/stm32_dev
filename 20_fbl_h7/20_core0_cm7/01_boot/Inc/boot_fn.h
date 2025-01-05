// port_disco_svc.h

#ifndef BOOT_FN_H
#define BOOT_FN_H

#ifdef __cplusplus
extern "C" {
#endif

void show_logo(void);
/**
 * @brief Jumps to app address
 */
void boot_jump_app(void);

#ifdef __cplusplus
}
#endif

#endif // BOOT_FN_H
