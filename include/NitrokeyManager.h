#ifndef LIBNITROKEY_NITROKEYMANAGER_H
#define LIBNITROKEY_NITROKEYMANAGER_H

#include "device.h"
#include "log.h"
#include "device_proto.h"
#include "stick10_commands.h"
#include "stick20_commands.h"
#include <vector>

namespace nitrokey {
    using namespace nitrokey::device;
    using namespace std;
    using namespace nitrokey::proto::stick10;
    using namespace nitrokey::proto::stick20;
    using namespace nitrokey::proto;
    using namespace nitrokey::log;

    class NitrokeyManager {
    public:
        static NitrokeyManager *instance();

        bool first_authenticate(const char *pin, const char *temporary_password);
        bool write_HOTP_slot(uint8_t slot_number, const char *slot_name, const char *secret, uint64_t hotp_counter,
                                     bool use_8_digits, const char *temporary_password);
        bool write_TOTP_slot(uint8_t slot_number, const char *slot_name, const char *secret,
                             uint16_t time_window, bool use_8_digits, const char *temporary_password);
        uint32_t get_HOTP_code(uint8_t slot_number, const char *user_temporary_password);
        uint32_t get_TOTP_code(uint8_t slot_number, uint64_t challenge, uint64_t last_totp_time, uint8_t last_interval,
                               const char *user_temporary_password);
        bool set_time(uint64_t time);
        bool get_time();
        bool erase_totp_slot(uint8_t slot_number, const char *temporary_password);
        bool erase_hotp_slot(uint8_t slot_number, const char *temporary_password);
        bool connect(const char *device_model);
        bool disconnect();
        void set_debug(bool state);
        string get_status();

        const char * get_totp_slot_name(uint8_t slot_number);
        const char * get_hotp_slot_name(uint8_t slot_number);

        void change_user_PIN(char *current_PIN, char *new_PIN);
        void change_admin_PIN(char *current_PIN, char *new_PIN);

        void enable_password_safe(const char *user_pin);

        uint8_t * get_password_safe_slot_status();

        uint8_t get_admin_retry_count();
        uint8_t get_user_retry_count();

        void lock_device();

        const char *get_password_safe_slot_name(uint8_t slot_number);
        const char *get_password_safe_slot_password(uint8_t slot_number);
        const char *get_password_safe_slot_login(uint8_t slot_number);

        void
    write_password_safe_slot(uint8_t slot_number, const char *slot_name, const char *slot_login,
                                 const char *slot_password);

        void erase_password_safe_slot(uint8_t slot_number);

        void user_authenticate(const char *user_password, const char *temporary_password);

        void factory_reset(const char *admin_password);

        void build_aes_key(const char *admin_password);

        void unlock_user_password(const char *admin_password);

        void write_config(bool numlock, bool capslock, bool scrolllock, bool enable_user_password,
                          bool delete_user_password, const char *admin_temporary_password);

        vector<uint8_t> read_config();

    private:
        NitrokeyManager();
        ~NitrokeyManager();

        static NitrokeyManager *_instance;
        bool connected;
        Device *device;

        bool is_valid_hotp_slot_number(uint8_t slot_number) const;
        bool is_valid_totp_slot_number(uint8_t slot_number) const;
        bool is_valid_password_safe_slot_number(uint8_t slot_number) const;
        uint8_t get_internal_slot_number_for_hotp(uint8_t slot_number) const;
        uint8_t get_internal_slot_number_for_totp(uint8_t slot_number) const;
        bool erase_slot(uint8_t slot_number, const char *temporary_password);
        uint8_t *get_slot_name(uint8_t slot_number) const;

        template <typename ProCommand, PasswordKind StoKind>
        void change_PIN_general(char *current_PIN, char *new_PIN);
    };
}



#endif //LIBNITROKEY_NITROKEYMANAGER_H
