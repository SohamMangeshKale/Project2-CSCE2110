# File:    tests/member_b_tests.sh
# Purpose: Tests for buildings/connections/departments loading, graph, BFS and the department tree.
# Author:  Member B
# Sourced by tests/run_tests.sh (needs the helpers in tests/test_lib.sh).
# Covers: LOAD (buildings, connections, departments), GRAPH, BFS, TREE

t L06 "Blank lines, comments, extra spaces OK; duplicate building differing by case rejected" $E '10\n' "WARNING: buildings.txt line 5: duplicate building name (ignoring case)"
t L07 "Building line with too many fields rejected" $E '10\n' "WARNING: buildings.txt line 7: wrong field count"
t L08 "Buildings summary on error file" $E '10\n' "buildings.txt: 5 accepted, 2 rejected"
t L09 "Reversed duplicate connection (student union|LIBRARY) rejected" $E '10\n' "WARNING: connections.txt line 3: duplicate connection"
t L10 "Exact duplicate connection rejected" $E '10\n' "WARNING: connections.txt line 4: duplicate connection"
t L11 "Self-connection rejected" $E '10\n' "WARNING: connections.txt line 5: self-connection not allowed"
t L12 "Unknown building in connections rejected, name reported" $E '10\n' "WARNING: connections.txt line 6: unknown building 'Atlantis'"
t L13 "Connection with too few fields rejected" $E '10\n' "WARNING: connections.txt line 7: wrong field count (expected 2, found 1)"
t L14 "Connection with too many fields rejected" $E '10\n' "WARNING: connections.txt line 8: wrong field count (expected 2, found 3)"
t L15 "Connection with empty field rejected" $E '10\n' "WARNING: connections.txt line 10: empty building name"
t L16 "Connections summary on error file" $E '10\n' "connections.txt: 2 accepted, 7 rejected"
t L23 "Department with unknown parent rejected" $E '10\n' "WARNING: departments.txt line 4: unknown parent 'Atlantis Dept'"
t L24 "Second root / duplicate-name root line rejected" $E '10\n' "WARNING: departments.txt line 5: root already exists"
t L25 "Department with too few fields rejected" $E '10\n' "WARNING: departments.txt line 7: wrong field count"
t L26 "Departments summary on error file" $E '10\n' "departments.txt: 3 accepted, 5 rejected"
t G01 "Graph display: isolated building shows (no connections)" $S '3\n10\n' "  Observatory: (no connections)" line
t G02 "Graph display: neighbors in insertion order" $S '3\n10\n' "  Student Union: Library, Science Building, Gym" line
t G03 "Counts: total buildings" $S '4\n10\n' "Total buildings: 7"
t G04 "Counts: connections counted once" $S '4\n10\n' "Total connections: 6"
s G05 "Degrees of every building" $S '4\n10\n' '^  [A-Za-z ]+: [0-9]+$' 's/^ +//' "Library: 2,Student Union: 3,Science Building: 2,Engineering Building: 1,Business Building: 2,Gym: 2,Observatory: 0"
t G06 "Rejected duplicate/reversed/self edges not counted (errors dataset)" $E '4\n10\n' "Total connections: 2"
t G07 "Degree after rejected reversed duplicate is 1 (Library)" $E '4\n10\n' "  Library: 1" line
s G08 "BFS from Library: visit order and distances" $S '5\nLibrary\n10\n' 'distance' 's/^ +//' "Library (distance 0),Student Union (distance 1),Business Building (distance 1),Science Building (distance 2),Gym (distance 2),Engineering Building (distance 3)"
t G09 "BFS from Library: reachable count includes start" $S '5\nLibrary\n10\n' "Reachable: 6 of 7 buildings"
s G10 "BFS from isolated Observatory returns only itself" $S '5\nObservatory\n10\n' 'distance' 's/^ +//' "Observatory (distance 0)"
t G11 "BFS from Observatory: reachable 1 of 7" $S '5\nObservatory\n10\n' "Reachable: 1 of 7 buildings"
t G12 "BFS unknown start: error shown" $S '5\nAtlantis\n10\n' "Error: unknown building 'Atlantis'."
t G13 "BFS unknown start: returns to menu and exits normally" $S '5\nAtlantis\n10\n' "Goodbye!"
t G14 "BFS start typed in lowercase with extra spaces" $S '5\n  student union  \n10\n' "Reachable: 6 of 7 buildings"
t G15 "Multiple components: Observatory not reachable from Engineering Building" $S '5\nEngineering Building\n10\n' "Reachable: 6 of 7 buildings"
t G16 "BFS blank building name rejected" $S '5\n\n10\n' "Error: building name cannot be blank."
t T01 "Empty tree: hierarchy shows empty message" testdata/emptytree '8\n10\n' "The department tree is empty."
t T02 "Empty tree: preorder shows empty message" testdata/emptytree '9\n10\n' "The department tree is empty."
t T03 "Empty tree: first department becomes root" testdata/emptytree '6\nUniversity\n10\n' "Added 'University' as the root department."
t T04 "Empty tree: no parent prompt for the root" testdata/emptytree '6\nUniversity\n10\n' "Enter parent department name" absent
t T05 "Root-only tree: hierarchy" testdata/rootonly '8\n10\n' "University" line
t T06 "Root-only tree: preorder" testdata/rootonly '9\n10\n' "Preorder traversal: University" line
t T07 "Search root: parent shown as None" $S '7\nUniversity\n10\n' "Parent: None"
t T08 "Search root: depth 0" $S '7\nUniversity\n10\n' "Depth: 0"
t T09 "Search non-root: parent" $S '7\nComputer Science\n10\n' "Parent: College of Engineering"
t T10 "Search non-root: depth 2" $S '7\nComputer Science\n10\n' "Depth: 2"
t T11 "Search department: case-insensitive, direct child count" $S '7\ncollege of engineering\n10\n' "Direct children: 2 (Computer Science, Electrical Engineering)"
t T12 "Search missing department" $S '7\nBiology\n10\n' "Not found: no department named 'Biology'."
t T13 "Add with unknown parent: error, tree unchanged" $S '6\nBiology\nNoSuchParent\n8\n10\n' "Error: unknown parent department 'NoSuchParent'. Tree unchanged."
t T14 "Add with unknown parent: new department not in hierarchy" $S '6\nBiology\nNoSuchParent\n8\n10\n' "    Biology" absent
t T15 "Add duplicate (case differs): rejected" $S '6\nphysics\n10\n' "Error: department 'physics' already exists"
t T16 "Add child then show indentation (depth 2 = 8 spaces)" $S '6\nChemistry\nCollege of Science\n8\n10\n' "        Chemistry" line
t T17 "Add department with spaces in name, then search it" $S '6\nSchool of Fine Arts\nUniversity\n7\nSchool of Fine Arts\n10\n' "Parent: University"
s T18 "Hierarchy: preorder with 4 spaces per level (shown as +)" $S '8\n10\n' '^( {4})*[A-Z][A-Za-z ]*$' 's/    /+/g' "University,+College of Engineering,++Computer Science,++Electrical Engineering,+College of Science,++Mathematics,++Physics"
t T19 "Preorder traversal sequence (no indentation)" $S '9\n10\n' "Preorder traversal: University -> College of Engineering -> Computer Science -> Electrical Engineering -> College of Science -> Mathematics -> Physics" line
t T20 "Added department appears in later preorder" $S '6\nChemistry\nCollege of Science\n9\n10\n' "College of Science -> Mathematics -> Physics -> Chemistry"
t T21 "Blank department name rejected" $S '6\n\n10\n' "Error: department name cannot be blank."
t T22 "Blank parent name rejected" $S '6\nNew Dept\n\n10\n' "Error: parent name cannot be blank."
t T23 "Blank name at department search rejected" $S '7\n\n10\n' "Error: department name cannot be blank."
