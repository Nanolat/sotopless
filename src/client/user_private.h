#ifndef USER_PRIVATE_H_
#define USER_PRIVATE_H_

/*! Private functions not exposed as SoTopless APIs, but used internally.
 * Using this function, the user identities can be retrieved by user handles.
 */
namespace topl {
    namespace user {
        /*! Get a mapping of user handles to user identities.
        * Using this function, the user identities can be retrieved by user handles.
        */
        result get( const std::vector<handle_t> user_handles,
                    std::map<handle_t, identity_t > * user_identity_map);
    }
}

#endif /* USER_PRIVATE_H_ */
