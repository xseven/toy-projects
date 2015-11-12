library(cluster)
library(fpc)

treesigmareduce <- function(x)
{
  if(!is.array(x))
  {
    return(x);
  }
  
  cols <- ncol(x);
  
  data <- array(, ñ());
  
  for (i in 1:cols)
  {
    column <- data[,i];
  }
}

clockData = read.csv("clocks.csv", , ";");

x <- unlist(clockData["X_WGS"]);
y <- unlist(clockData["Y_WGS"]);

#plot(x, y, xlab="", ylab="");

xydata = matrix(, length(x), 2);

xydata[, 1] <- x;
xydata[, 2] <- y;

km <- kmeans(xydata, 9);

for (i in 1:length(km$cluster))
{
  #points(x[i], y[i], col = km$cluster[i]+1);
}

hull <- c(3,2);
hull <- c(hull, c(37.4867430, 55.8153460));
hull <- c(hull, c(37.4881344, 55.8152801));
hull <- c(hull, c(37.4934952, 55.8151643));

