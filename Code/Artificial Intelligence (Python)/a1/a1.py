import random
import math
import numpy as np
import string
import matplotlib.pyplot as plt
import heapq as hq
import sys


## variables of the map
map = [[] for x in range(26)]
distance = np.zeros((26, 26))
edges = np.zeros((26, 26))
cities = ['Athens','Berlin','Copenhagen','Dublin','Edmonton','Florence','Getafe','Helsinki','Istanbul','Jakarta','Kiev','Lisbon','Moscow','New York',
'Oslo','Paris','Quebec','Rome','Stockholm','Tallinn','Utrecht','Vienna','Warsaw','Xalapa','Yokohoma','Zagreb']

## creates a 100x100 2 dimensional grid
def board():
	for city in range(len(map)):
		axis = [random.randint(0, 99), random.randint(0, 99)]
		while axis in map:
			axis = [random.randint(0, 99), random.randint(0, 99)]
		map[city] = axis

## creates distances for the cities
def euclideanDistance():
	for cityLocation in range(len(map)):
		for neighbour in range(cityLocation+1, len(map)):
			unitDistance = math.sqrt((map[cityLocation][0] - map[neighbour][0])**2 + (map[cityLocation][1] - map[neighbour][1])**2)
			distance[cityLocation][neighbour] = unitDistance
			distance[neighbour][cityLocation] = unitDistance

# creates the roads between cities
def Roads():
	global roadsBetweenCities
	roadsBetweenCities= np.zeros((26,26))
	totalEdges = 0.0
	for city in range(len(distance)):
		numberofEdges = random.randint(1, 4)
		totalEdges += numberofEdges
		neighbourCity = np.argsort(distance[city])[:numberofEdges + 1][1:]
		for neighbour in neighbourCity:
			edges[city][neighbour.item()] = distance[city][neighbour.item()]
			edges[neighbour.item()][city] = distance[city][neighbour.item()]

	print "The average number of edges:" + str(totalEdges / 26)

#BFS algorithm
def BFS(startLocation, destinationLocation):

	findPath = [[startLocation]]
	visitedCities = [0 for x in range(26)]
	citiesVisited =1
	total=0
	while findPath:
		pathFound = findPath.pop(0)
		findTheCity = pathFound[-1]
		visitedCities[findTheCity] = 1
		total += 1
		neighbours = [i for i, e in enumerate(edges[findTheCity]) if e != 0]
		for neighbour in neighbours:
			findRoad = pathFound[:]
			findRoad.append(neighbour)
			if neighbour == destinationLocation:
				return findRoad,citiesVisited,total
			else:
				if visitedCities[neighbour]:
					continue
				else:
					findPath.append(findRoad)
					citiesVisited += 1

	return [],citiesVisited,total

#DFS algorithm
def DFS(startLocation, destinationLocation):

	findPath = [[startLocation]]
	visitedCities = [0 for x in range(26)]
	citiesVisited =1
	total=0
	while findPath:
		pathFound = findPath.pop()
		findTheCity = pathFound[-1]
		visitedCities[findTheCity] = 1
		total += 1
		neighbours = [i for i, e in enumerate(edges[findTheCity]) if e != 0]
		for neighbour in neighbours:
			findRoad = pathFound[:]
			findRoad.append(neighbour)
			if neighbour == destinationLocation:
				return findRoad, visitedCities,total
			else:
				if visitedCities[neighbour]:
					continue
				else:
					findPath.append(findRoad)
					citiesVisited += 1
	return [],total,citiesVisited

# IDDFS algorithm
def IDDFS(startLocation, destinationLocation):

	total =0
	citiesVisited=1
	for depth in range(26):
		findPath = [[startLocation]]
		while findPath:
			visitedCities = [0 for x in range(26)]
			pathFound = findPath.pop()
			findTheCity = pathFound[-1]
			total += 1
			visitedCities[findTheCity] = 1
			neighbours = [i for i, e in enumerate(edges[findTheCity]) if e != 0]
			for neighbour in neighbours:
				findRoad = pathFound[:]
				findRoad.append(neighbour)
				if neighbour == destinationLocation:
					return findRoad ,total,citiesVisited
				else:
					if visitedCities[neighbour]:
						continue
					else:
						if len(findRoad) <= depth:
							findPath.append(findRoad)
							citiesVisited += 1

	return [],citiesVisited,total

#GBFS algorithm
def GBFS(startLocation, destinationLocation):
	##greedy
	foundTheRoad = []
	hq.heappush(findPath,(distance[startLocation][destinationLocation],[startLocation]))
	total =1
	citiesVisited=0
	visitedCities=[0 for x in range(26)]



	while findPath:

			pathFound = hq.heappop(findPath)[1]
			citiesVisited +=1
			findTheCity= pathFound[-1]

			if findTheCity ==destinationLocation:
				return pathFound,total,citiesVisited

			visitedCities[findTheCity]=1
			neighbours=np.argsort(edges[findTheCity])[-np.count_nonzero(edges[findPath]):]

			for neighbour in neighbours:
				findRoad = pathFound[:]
				findRoad.append(neighbour)
				lastRoad=distance[neighbour][destinationLocation]
				if visitedCities[neighbour]:
					continue
				hq.heappush(findPath,(lastRoad,findRoad))
				total += 1



	return [], total, citiesVisited

# A* algorithm
def Astar(startLocation, destinationLocation):
	## A* search

	findPath = []
	hq.heappush(findPath, (distance[startLocation][destinationLocation],0,[startLocation]))
	citiesVisited =1
	total =0
	visitedCities = [0 for x in range(26)]

	while findPath:
		pathFound = hq.heappop(findPath)[1]
		findTheCity = pathFound[-1]
		total +=1
		firstRoad=pathFound[2]
		findTheCity=firstRoad[-1]

		if findTheCity == destinationLocation:
				return firstRoad,total,citiesVisited

		visitedCities[findTheCity] = 1
		neighbourCity = np.argsort(edges[findTheCity])[-np.count_nonzero(edges[findTheCity]):]

		for neighbour in neighbourCity:
			findRoad = pathFound[:]
			findRoad.append(neighbour)
			RemaningDistance = distance[neighbour][destinationLocation]
			remaningCost=pathFound[1]+ edges[findTheCity][neighbour]

			if visitedCities[neighbour]:
				continue
			hq.heappush(findPath, (RemaningDistance+remaningCost,remaningCost,findRoad ))
			citiesVisited +=1
	return [],total,citiesVisited

#calculates the total distance
def totalCost():

	if not roadFound:
		return None

	firstRoad=roadFound.pop(0)
	total=0

	while roadFound:
		nextRoad = roadFound.pop(0)
		total += edges[firstRoad][nextRoad]
		firstRoad= nextRoad


	return total


# calls the board functions
def setup():

	board()
	euclideanDistance()
	Roads()

# prints and calculates stat
def main():

	setup()

	startLocation = random.randint(0, 25)
	destinationLocation = random.randint(0, 25)

	while destinationLocation == startLocation:
		destinationLocation = random.randint(0, 25)

	bfs = BFS(startLocation, destinationLocation)[0]
	dfs = DFS(startLocation, destinationLocation)[0]
	iddfs = IDDFS(startLocation, destinationLocation)[0] if BFS is not None else None
	gbfs = GBFS(startLocation, destinationLocation)[0]
	Astar= Astar(startLocation,destinationLocation)[0]


	print "Start Location: " + cities[startLocation] + "\nDestination Location: " + cities[destinationLocation]
	print "BFS:", [cities[x] for x in bfs] if bfs else None,"\n\tpath cost:", totalCost(BFS)
	print "DFS:", [cities[x] for x in dfs] if dfs else None,"\n\tpath cost:", totalCost(DFS)
	print "IDDFS:", [cities[x] for x in iddfs] if iddfs else None,"\n\tpath cost:", totalCost(IDDFS)
	print "GBFS:", [cities[x] for x in gbfs] if gbfs else None,"\n\tpath cost:", totalCost(GBFS)
    print "A*:", [cities[x] for x in Astar] if Astar else None,"\n\tpath cost:", totalCost(Astar)

	spaceComplexityBFS = 0
	spaceComplexityDFS = 0
	spaceComplexityIDDFS = 0
	spaceComplexityGBFS = 0
	spaceComplexityAstar = 0
	TimeComplexityBFS = 0
	TimeComplexityDFS = 0
	TimeComplexityIDDFS = 0
	TimeComplexityGBFS = 0
	TimeComplexityAstar = 0
	RunningTimeBFS = 0.0
	RunningTimeDFS = 0.0
	RunningTimeIDDFS = 0.0
	RunningTimeGBFS = 0.0
	RunningTimeAstar = 0.0
	distanceBFS = 0
	DistanceDFS = 0
	DistanceIDDFS = 0
	DistanceGBFS = 0
	DistanceAstar = 0
	ProblemSolvedBFS = 0
	problemSolvedDFS = 0
	problemSolvedIDDFS = 0
	problemSolvedGBFS = 0
	problemSolvedAstar = 0


	for x in range(0, 100):
		Roads()
		startLocation = random.randint(0, 25)
		destinationLocation = random.randint(0, 25)

		while destinationLocation == startLocation:
			destinationLocation = random.randint(0, 25)

		BFS, BFSfound, BFSlist =BFS(startLocation, destinationLocation)
		spaceComplexityBFS += BFSfound
		TimeComplexityBFS += BFSlist
		DistanceBFS += len(BFS)
		if BFS:
			number_of_problems_solved_bfs += 1

		DFS, DFSfound, DFSlist = DFS(startLocation, destinationLocation)
		spaceComplexityDFS += DFSfound
		TimeComplexityDFS += DFSlist
		DistanceDFS += len(DFS)
		if DFS:
			problemSolvedDFS += 1

		IDDFS, IDDFSfound, idDFSlist = IDDFS(startLocation, destinationLocation)
		spaceComplexityIDDFS += IDDFSfound
		TimeComplexityIDDFS += idDFSlist
		DistanceIDDFS += len(IDDFS)
		if IDDFS:
			problemSolvedIDDFS += 1

		GBFS, gBFSfound, gBFSlist = GBFS(startLocation, destinationLocation)
		spaceComplexityGBFS += gBFSfound
		TimeComplexityGBFS += gBFSlist
		DistanceGBFS += len(GBFS)
		if GBFS:
			problemSolvedGBFS += 1

		Astar, AstarFound, AstarList = Astar(startLocation, destinationLocation)
		spaceComplexityAstar += AstarFound
		TimeComplexityAstar += AstarList
		DistanceAstar += len(Astar)
		if Astar:
			problemSolvedAstar += 1

	spaceComplexityBFS = spaceComplexityBFS / 100
	spaceComplexityDFS = spaceComplexityDFS / 100
	spaceComplexityIDDFS = spaceComplexityIDDFS / 100
	spaceComplexityGBFS = spaceComplexityGBFS / 100
	spaceComplexityAstar = spaceComplexityAstar / 100

	TimeComplexityBFS = TimeComplexityBFS / 100
	TimeComplexityDFS = TimeComplexityDFS / 100
	TimeComplexityIDDFS = TimeComplexityIDDFS / 100
	TimeComplexityGBFS = TimeComplexityGBFS / 100
	TimeComplexityAstar = TimeComplexityAstar / 100

	RunningTimeBFS = RunningTimeBFS / 100
	RunningTimeDFS = RunningTimeDFS / 100
	RunningTimeIDDFS = RunningTimeIDDFS / 100
	RunningTimeGBFS = RunningTimeGBFS / 100
	RunningTimeAstar = RunningTimeAstar / 100

	DistanceBFS = DistanceBFS / 100
	DistanceDFS = DistanceDFS / 100
	DistanceIDDFS = DistanceIDDFS / 100
	DistanceGBFS = DistanceGBFS / 100
	DistanceAstar = DistanceAstar / 100

	print "----------- BFS ---------------"
	print "BFS average space complexity:", spaceComplexityBFS
	print "BFS average time complexity:", TimeComplexityBFS
	print "BFS average running time:", RunningTimeBFS
	print "BFS average path length:", DistanceBFS
	print "BFS number of problems solved:", number_of_problems_solved_bfs, "\n"

	print "----------- DFS --------------"
	print "DFS average space complexity:", spaceComplexityDFS
	print "DFS average time complexity:", TimeComplexityDFS
	print "DFS average running time:", RunningTimeDFS
	print "DFS average path length:", DistanceDFS
	print "DFS number of problems solved:", problemSolvedDFS, "\n"

	print "---------- IDDFS ---------------"
	print "IDDFS average space complexity:", spaceComplexityIDDFS
	print "IDDFS average time complexity:", TimeComplexityIDDFS
	print "IDDFS average running time:", RunningTimeIDDFS
	print "IDDFS average path length:", DistanceIDDFS
	print "IDDFS number of problems solved:", problemSolvedIDDFS, "\n"

	print "---------- GBFS ---------------"
	print "GBFS average space complexity:", spaceComplexityGBFS
	print "GBFS average time complexity:", TimeComplexityGBFS
	print "GBFS average running time:", RunningTimeGBFS
	print "GBFS average path length:", DistanceGBFS
	print "GBFS number of problems solved:", problemSolvedGBFS, "\n"

	print "---------- Astar -----------"
	print "Astar average space complexity:", spaceComplexityAstar
	print "Astar average time complexity:", TimeComplexityAstar
	print "Astar average running time:", RunningTimeAstar
	print "Astar average path length:", DistanceAstar
	print "Astar number of problems solved:", problemSolvedAstar, "\n"



if __name__ == '__main__':
    main()
