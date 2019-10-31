from kanren import Relation, facts
from kanren import run, eq, membero, var, conde

def I():

    parent = Relation()
    facts(parent, 
        ("Darth Vader", "Luke Skywalker"),
        ("Darth Vader", "Leia Organa"),
        ("Leia Organa",  "Kylo Ren"),
        ("Han Solo",  "Kylo Ren"))
    x = var()
    print(run(1, x, parent(x, "Luke Skywalker")))
    y = var()
    print(run(2, y, parent("Darth Vader", y)))

    grand_parents = Relation()
    facts(grand_parents,
        ("Darth Vader", "Kylo Ren"))
    z = var()
    print(run(1, z, grand_parents(z, "Kylo Ren")))


def findParentIndices(relations, member_name, family_members):
    child_index = family_members.index(member_name)
    parent_indices = [
        relation[0] for relation in relations if relation[1] == child_index]
    return set(parent_indices)


def findChildrenIndices(relations, member_name, family_members):
    parent_index = family_members.index(member_name)
    children_indices = [
        relation[1] for relation in relations if relation[0] == parent_index]
    return set(children_indices)


def printMembersGivenIndices(family_members, indices):

    for index in indices:
        print(family_members[index])


def II():

    family_members = [
        "Darth Vader", "Luke Skywalker", "Leia Organa", "Kylo Ren"]
    
    relations = [
        (0, 1), (0, 2), (1, 3), (2, 3)]

    child = "Luke Skywalker"
    print("\n")
    print("Parent of %s: " % child)
    parent_indices = findParentIndices(relations, child, family_members)
    printMembersGivenIndices(family_members, parent_indices)

    parent = "Darth Vader"
    print("\n")
    print("Children of %s: " % parent)
    children_indices = findChildrenIndices(relations, parent, family_members)
    printMembersGivenIndices(family_members, children_indices)  

    grand_children = "Kylo Ren"
    print("\n")
    print("Grandparents of %s: " % grand_children)
    parent_indices = findParentIndices(relations, grand_children, family_members)
    grand_parent_indices = []
    for parent_index in parent_indices:
        parent_indices = findParentIndices(relations, family_members[parent_index], family_members)
        grand_parent_indices.extend(parent_indices)
    grand_parent_indices = set(grand_parent_indices)
    printMembersGivenIndices(family_members, grand_parent_indices)
    

def main():
    II()
    

if __name__ == '__main__':
    main()
