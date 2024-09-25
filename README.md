## asiant

AI functionality based on the book "AI for Games" by Ian Millington.

To build and execute just type `make` and then `./build/demos/seek/seek`, for instance.

I will list points here that need addressing:

- [ ] Major refactor of seek class. I think we need to make a more general base class to get wander, seek, flee etc. all
under the same ABC. Maybe call the ABC 'movement' or something like that.

- [ ] While `vector`, `utils` and `timer` are tested, most of the other components are not. Fix that.
- [ ] Update old demos to use pi etc. from `utils`.
= [ ] Add random number generation to utils.
