 can we make a consteval (forced to compute at compile time) function that computes pi to the Nth digi
 some other const notes:
 - with a regular const value we can std::const_cast it back to be non_const
 consteval cannot be used for variables, only for functions
 constexpr can be used for both functions and variables, but does not guarantee it will run at compile time funnction, this forces the variable to have a value at compile time or will fail
 BUT! this does not make them const, they can still be changed