// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef _ACCOUNT_HPP_
#define _ACCOUNT_HPP_

#include "../common/cbasetypes.hpp"
#include "../common/mmo.hpp" // ACCOUNT_REG2_NUM
#include "../config/core.hpp"

// (^~_~^) Gepard Shield Start

#include "../common/socket.hpp"

void account_gepard_update_last_unique_id(int account_id, unsigned int unique_id);
bool account_gepard_check_unique_id(int fd, struct socket_data* s);
int account_gepard_check_license_version(struct socket_data* s, int fd, int group_id);

// (^~_~^) Gepard Shield End
typedef struct AccountDB AccountDB;
typedef struct AccountDBIterator AccountDBIterator;


// standard engines
AccountDB* account_db_sql(void);

struct mmo_account {
	uint32 account_id;
	char userid[NAME_LENGTH];
	char pass[32+1];        // 23+1 for plaintext, 32+1 for md5-ed passwords
	char sex;               // gender (M/F/S)
	char email[40];         // e-mail (by default: a@a.com)
	unsigned int group_id;  // player group id
	uint8 char_slots;       // this accounts maximum character slots (maximum is limited to MAX_CHARS define in char server)
	unsigned int state;     // packet 0x006a value + 1 (0: compte OK)
	time_t unban_time;      // (timestamp): ban time limit of the account (0 = no ban)
	time_t expiration_time; // (timestamp): validity limit of the account (0 = unlimited)
	unsigned int logincount;// number of successful auth attempts
	char lastlogin[24];     // date+time of last successful login
	char last_ip[16];       // save of last IP of connection
	char birthdate[10+1];   // assigned birth date (format: YYYY-MM-DD)
	char pincode[PINCODE_LENGTH+1];		// pincode system
	time_t pincode_change;	// (timestamp): last time of pincode change
#ifdef VIP_ENABLE
	int old_group;
	time_t vip_time;
#endif
};


struct AccountDBIterator {
	/// Destroys this iterator, releasing all allocated memory (including itself).
	///
	/// @param self Iterator
	void (*destroy)(AccountDBIterator* self);

	/// Fetches the next account in the database.
	/// Fills acc with the account data.
	/// @param self Iterator
	/// @param acc Account data
	/// @return true if successful
	bool (*next)(AccountDBIterator* self, struct mmo_account* acc);
};


struct AccountDB {
	/// Initializes this database, making it ready for use.
	/// Call this after setting the properties.
	///
	/// @param self Database
	/// @return true if successful
	bool (*init)(AccountDB* self);

	/// Destroys this database, releasing all allocated memory (including itself).
	///
	/// @param self Database
	void (*destroy)(AccountDB* self);

	/// Gets a property from this database.
	/// These read-only properties must be implemented:
	///
	/// @param self Database
	/// @param key Property name
	/// @param buf Buffer for the value
	/// @param buflen Buffer length
	/// @return true if successful
	bool (*get_property)(AccountDB* self, const char* key, char* buf, size_t buflen);

	/// Sets a property in this database.
	///
	/// @param self Database
	/// @param key Property name
	/// @param value Property value
	/// @return true if successful
	bool (*set_property)(AccountDB* self, const char* key, const char* value);

	/// Creates a new account in this database.
	/// If acc->account_id is not -1, the provided value will be used.
	/// Otherwise the account_id will be auto-generated and written to acc->account_id.
	///
	/// @param self Database
	/// @param acc Account data
	/// @return true if successful
	bool (*create)(AccountDB* self, struct mmo_account* acc);

	/// Removes an account from this database.
	///
	/// @param self Database
	/// @param account_id Account id
	/// @return true if successful
	bool (*remove)(AccountDB* self, const uint32 account_id);

	/// Modifies the data of an existing account.
	/// Uses acc->account_id to identify the account.
	///
	/// @param self Database
	/// @param acc Account data
	/// @return true if successful
	bool (*save)(AccountDB* self, const struct mmo_account* acc);

	/// Finds an account with account_id and copies it to acc.
	///
	/// @param self Database
	/// @param acc Pointer that receives the account data
	/// @param account_id Target account id
	/// @return true if successful
	bool (*load_num)(AccountDB* self, struct mmo_account* acc, const uint32 account_id);

	/// Finds an account with userid and copies it to acc.
	///
	/// @param self Database
	/// @param acc Pointer that receives the account data
	/// @param userid Target username
	/// @return true if successful
	bool (*load_str)(AccountDB* self, struct mmo_account* acc, const char* userid);

	/// Returns a new forward iterator.
	///
	/// @param self Database
	/// @return Iterator
	AccountDBIterator* (*iterator)(AccountDB* self);
};

void mmo_send_global_accreg(AccountDB* self, int fd, int account_id, int char_id);
void mmo_save_global_accreg(AccountDB* self, int fd, int account_id, int char_id);


#endif /* _ACCOUNT_HPP_ */