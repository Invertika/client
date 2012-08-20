var Error =
{ 
		// Generic return values
		ERRMSG_OK:0,                      // everything is fine
		ERRMSG_FAILURE:1,                     // the action failed
		ERRMSG_NO_LOGIN:2,                    // the user is not yet logged
		ERRMSG_NO_CHARACTER_SELECTED:3,     // the user needs a character
		ERRMSG_INSUFFICIENT_RIGHTS:4,         // the user is not privileged
		ERRMSG_INVALID_ARGUMENT:5,          // part of the received message was invalid
		ERRMSG_EMAIL_ALREADY_EXISTS:6,       // The Email Address already exists
		ERRMSG_ALREADY_TAKEN:7,             // name used was already taken
		ERRMSG_SERVER_FULL:8,                // the server is overloaded
		ERRMSG_TIME_OUT:9,                   // data failed to arrive in due time
		ERRMSG_LIMIT_REACHED:10,            // limit reached
		ERRMSG_ADMINISTRATIVE_LOGOFF:11       // kicked by server administrator
}