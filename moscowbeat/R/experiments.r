library(cluster)
library(fpc)

clockData = read.csv("clocks.csv", , ";");

x <- unlist(clockData["X_WGS"]);
y <- unlist(clockData["Y_WGS"]);

plot(x, y, xlab="", ylab="");

xydata = matrix(, length(x), 2);

xydata[, 1] <- x;
xydata[, 2] <- y;

km <- kmeans(xydata, 12);

plotcluster(xydata, km$cluster);