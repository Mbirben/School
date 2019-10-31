import matplotlib.pyplot as plt

# mapFigFilePath = "targets/img/map.png"
# distFigFilePath = "targets/pdf/distance.pdf"

class GraphUtils:

    def generateCityGraph(gridStats, path, mapFigFilePath):

        keys = ["cities", "roads", "startState", "goalState", "roadsWDist", "avgBrFctr"]
        cities = gridStats[keys[0]]
        roads = gridStats[keys[1]]
        sLocation = gridStats[keys[2]]
        eLocation = gridStats[keys[3]]
        roadsWDist = gridStats[keys[4]]

        if len(cities) == 0:
            return

        # plot cities
        xCoors = []
        yCoors = []
        labels = []
        for index in range(len(cities)):
            city = cities[index]
            xCoors.append(city.xCoor)
            yCoors.append(city.yCoor)
            labels.append(city.label)
        plt.plot(xCoors, yCoors, 'ro')

        plt.annotate("%s (start)" % sLocation.label, xy=(sLocation.xCoor, sLocation.yCoor), textcoords='data')
        plt.annotate("%s (end)" % eLocation.label, xy=(eLocation.xCoor, eLocation.yCoor), textcoords='data')

        for city in path:
            plt.annotate(city.label, xy=(city.xCoor, city.yCoor), textcoords='data')

        # plot roads between cities
        for i in range(len(cities)):
            for j in range(i + 1, len(cities)):
                if roads[i][j] == 1:
                    xCoors = []
                    yCoors = []
                    iCity = cities[i]
                    jCity = cities[j]
                    xCoors = [iCity.xCoor, jCity.xCoor]
                    yCoors = [iCity.yCoor, jCity.yCoor]
                    plt.plot(xCoors, yCoors,'r-', alpha=0.2)

        # plot path
        for i in range(len(path)):
            if i == len(path) - 1:
                break
            j = i + 1
            xCoors = []
            yCoors = []
            iCity = path[i]
            jCity = path[j]
            xCoors = [iCity.xCoor, jCity.xCoor]
            yCoors = [iCity.yCoor, jCity.yCoor]
            plt.plot(xCoors, yCoors,'g-')

        plt.axis('off')
        plt.savefig(mapFigFilePath)
        plt.close()

    def generateDistanceTable(gridStats, distFigFilePath):

        keys = ["cities", "roads", "startState", "goalState", "roadsWDist", "avgBrFctr"]
        roadsWDist = gridStats[keys[4]]
        cellTexts = []
        rows = ["Starting Location", "End Location", "Distance"]
        keys = ["sCity", "eCity", "distance"]
        cols = None
        for index in range(len(roadsWDist)):
            obj = roadsWDist[index]
            cellText = [obj[keys[0]].label, obj[keys[1]].label, obj[keys[2]]]
            cellTexts.append(cellText)

        hcell, wcell = 0.1, 0.1 # tweak as per your requirements
        hpad, wpad = 0.5, 0.5
        ncols = 3
        nrows = len(roadsWDist)
        # fig=plt.figure(figsize=(ncols*wcell+wpad, nrows*hcell+hpad))
        fig=plt.figure(figsize=(ncols, nrows))
        table = plt.table(cellText=cellTexts, rowLabels=None, colLabels=rows, loc='center')
        plt.axis('off')
        plt.savefig(distFigFilePath)
        plt.close()


    def generateDistanceTableI(gridStats, distList, distFigFilePath):

        keys = ["cities", "roads", "startState", "goalState", "roadsWDist", "avgBrFctr"]
        goalCity = gridStats[keys[3]]
        cellTexts = []
        rows = ["Starting Location", "End Location", "Distance"]
        cols = None
        for index in range(len(distList)):
            obj = distList[index]
            cellText = [obj[0].label, goalCity.label, obj[1]]
            cellTexts.append(cellText)

        hcell, wcell = 0.1, 0.1 # tweak as per your requirements
        hpad, wpad = 0.5, 0.5
        ncols = 3
        nrows = len(distList)
        # fig=plt.figure(figsize=(ncols*wcell+wpad, nrows*hcell+hpad))
        fig=plt.figure(figsize=(ncols, nrows))
        table = plt.table(cellText=cellTexts, rowLabels=None, colLabels=rows, loc='center')
        plt.axis('off')
        plt.savefig(distFigFilePath)
plt.close()
