using namespace eosio;
using std::string;

class recipient : public eosio::contract {
public:
    using contract::contract;

    void transfer(account_name from, account_name to, asset quantity, string memo) {
      print(name{from}, " transfer to ", name{to}, "  ", quantity, " memo:", memo);

      // memo cannot be empty
      check(memo.size() > 0, "memo cannot be empty");

      // memo must be digit
      for (int i = 0; i < memo.length(); i++) {
          check(isdigit(memo[i]) != 0, "invalid memo")
      }   
    }   
};

extern "C" {
    void apply(uint64_t receiver, uint64_t code, uint64_t action) {
        if (code == "eosio.token"_n.value && action == "transfer"_n.value) {
            recipient tc(receiver);
            const currency::transfer &t = unpack_action_data<transfer>();
            tc.transfer(t.from, t.to, t.quantity, t.memo);
        }   
        eosio_exit(0);
    }   
}
~   
