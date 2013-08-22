#include "xUnit++/xUnit++.h"
#include <topless.h>
#include "test_common.h"
using namespace topl;



THEORY("Create/Get User Should Succeed", (const std::string & user_identity),
   std::make_tuple("a"),
   std::make_tuple("a1"),
   std::make_tuple("a01"),
   std::make_tuple("1"),
   std::make_tuple("1a"),
   std::make_tuple("01"),
   std::make_tuple("01a"),
   std::make_tuple("foo.bar@gmail.com"),
   std::make_tuple("2b6f0cc904d137be2e1730235f5664094b831186"), // device-id
   std::make_tuple("759bfd0a-2c91-4e45-a35c-779740ac5e77") // GUID
)
{
    user::handle_t searched_handle;

    // The user does not exist yet. user::get should fail.
    ASSERT_FAILURE( user::get(user_identity, &searched_handle) );

    user::handle_t created_handle;
    ASSERT_SUCCESS( user::create(user_identity, &created_handle) );

    searched_handle = 0;
    ASSERT_SUCCESS( user::get(user_identity, &searched_handle) );

    Assert.Equal( created_handle, searched_handle );
}


THEORY("Create User with Invalid Identiy Should Fail", (const std::string & user_identity),
   std::make_tuple("")
)
{
    user::handle_t created_handle;

    ASSERT_FAILURE( user::create(user_identity, &created_handle) );
}
