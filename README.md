## asiant

AI functionality based on the book "AI for Games" by Ian Millington.

My code improves on that suggested in the book in the following ways:
- The path following algorithm in the book is O(n). I get away with a O(1) implementation by getting the character to
follow a path constrained entity.

To build and execute just type `make` and then `./build/demos/seek/seek`, for instance.

I will list points here that need addressing:

- [ ] Major refactor of seek class. I think we need to make a more general base class to get wander, seek, flee etc. all
under the same ABC. Maybe call the ABC 'movement' or something like that.

- [ ] While `vector`, `utils` and `timer` are tested, most of the other components are not. Fix that.
- [ ] Update old demos to use pi etc. from `utils`.
- [ ] Add random number generation to utils.
- [ ] path_constrained_entity suffers from variable speeds that depend on line_segment length. Fix this.
