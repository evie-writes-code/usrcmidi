# usrcmidi
ulrike's pure data source repositories, midi handling

## Documentation

### mono
Companion to the **poly** object in vanilla Pd, implements the common voice stealing behaviour of mono synthesisers.
Takes [note, velocity( messages. On receiving a note-on/nonzero-velocity message, that note is added to the top of the stack,
becoming the new current note. On receiving a note-off/zero-velocity message, the stack is searched for instances
of that note and all instances are removed. If one of those instances was the previous top note, the current note becomes
the new top note on the stack.
Every time the current note changes, a note-on message is sent to the outlet of the new current note.
If the stack becomes empty as a result of a note-off message, a note off-message is sent.

### indexToMode
Converts scale degrees to their actual midi values given a specified mode. Takes a scale degree and mode as argument and returns the midi value. Midi values are indexed from 0 so one can add a certain midi note value to make the whole thing in that key, scale indices indexed from 1.
e.g.
```
degree	mode        midi
1       ionian      0
2       ionian      2
3       ionian      4
4       ionian      5
5       ionian      7

1       phrygian    0
2       phrygian    1
3       phrygian    3
```