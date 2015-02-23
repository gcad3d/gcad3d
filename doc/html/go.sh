# input:  index_tree_de.htm.htm
# output: index_tree_de.htm
# make index_tree_en.htm manually !

#cc -o htmTree htmTree.c

./htmTree index_tree_de && firefox index_de.htm
./htmTree index_tree_en && firefox index_en.htm

# eof
