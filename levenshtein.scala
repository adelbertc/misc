// Based off C++ implementation here:
// http://www.merriampark.com/ldcpp.htm

object Levenshtein {
  def getArray(n: Int, m: Int): Array[Array[Int]] = {
    val retArray = new scala.collection.mutable.ListBuffer[Array[Int]]
    for (i <- 0 to n) retArray += ((0 to m) toArray)
    retArray.toArray
  }

  def levenshteinDistance(source: String, target: String): Int = {
    val n = source.length
    val m = target.length

    if (n == 0) return m
    if (m == 0) return n

    val distanceMatrix = getArray(n, m)
    
    for (i <- 0 to n) distanceMatrix(i)(0) = i
    for (j <- 0 to m) distanceMatrix(0)(j) = j

    for (i <- 1 to n) {
      var si = source(i - 1)

      for (j <- 1 to m) {
        val tj = target(j - 1)
        var cost = 0

        if (si == tj) cost = 0
        else cost = 1

        val above = distanceMatrix(i - 1)(j)
        val left = distanceMatrix(i)(j - 1)
        val diag = distanceMatrix(i - 1)(j - 1)
        var cell = (above + 1).min((left + 1).min(diag + cost))

        if (i > 2 && j > 2) {
          var trans = distanceMatrix(i - 2)(j - 2) + 1
          if (source(i - 2) != tj) trans += 1
          if (si != target(j - 2)) trans += 1
          if (cell > trans) cell = trans
        }

        distanceMatrix(i)(j) = cell
      }
    }
    distanceMatrix(n)(m)
  }

  def main(args: Array[String]) {
    if (args.length != 2) {
      println("Usage: scala Levenshtein [source] [target]")
      return
    }

    val source = args(0)
    val target = args(1)

    print("Levenshtein distance between " + source + " and " + target)
    println(" is: " + levenshteinDistance(source, target))
  }
}
