#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/dispatcher.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/asset.hpp>
using namespace eosio;
using std::string;

class recipient : public eosio::contract {
public:
    using contract::contract;

    void transfer(name from, name to, asset quantity, string memo) {
      print(name{from}, " transfer to ", name{to}, "  ", quantity, " memo:", memo);

      // memo cannot be empty
      check(memo.size() > 0, "memo cannot be empty");

      // memo must be digit
      for (int i = 0; i < memo.length(); i++) {
          check(isdigit(memo[i]) != 0, "invalid memo");
      }   
    }   
};

struct temptransfer{
  name    from;
  name    to;
  asset   quantity;
  std::string  memo;
  
  EOSLIB_SERIALIZE(temptransfer,(from)(to)(quantity)(memo));
};

extern "C" {
    void apply(uint64_t receiver, uint64_t code, uint64_t action) {
        if (code == "eosio.token"_n.value && action == "transfer"_n.value) {

            size_t size = action_data_size();
            constexpr size_t max_stack_buffer_size = 512;
            void* buffer = nullptr;
            if( size > 0 ) {
               buffer = max_stack_buffer_size < size ? malloc(size) : alloca(size);
               read_action_data( buffer, size );
            }
            datastream<const char*> ds((char*)buffer, size);
            
            recipient tc(eosio::name(receiver), eosio::name(code), ds);
            const temptransfer& t = unpack_action_data<temptransfer>();
            tc.transfer(t.from, t.to, t.quantity, t.memo);
        }   
        eosio_exit(0);
    }   
}
