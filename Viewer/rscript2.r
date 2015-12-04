# rscript2.r(Time Plot+Simple Bar Plot)
# Output Option('-o')
fun_output <- function(){
	flag = FALSE
	flag2 = FALSE
	args <- commandArgs(trailingOnly = TRUE)

	# Graph Size Option('-s')
	x_length = 7
	y_length = 7
	if(length(args) != 0){
		for(i in 1:length(args)){
			if(args[i] == '-s'){
				x_length = as.double(args[i+1])
				y_length = as.double(args[i+2])
			}
		}
	}
	# Input Option('-i') - Diretory	
	if(length(args) != 0){
		for(i in 1:length(args)){
			if(args[i] == '-i'){
				flag2 = TRUE
				name = args[i+1]
				time = args[i+2]
				dir = paste(name, time, sep="-")	
			}
		}
	}
	if(length(args) != 0){
        	for(i in 1:length(args)){
                	if(args[i] == '-o'){
                        	flag = TRUE
				if(flag2 == TRUE)
					dir = paste(dir, args[i+1], sep="/")
				else
					dir = paste("save", args[i+1], sep="/")
                        	if(args[i+2] == 'pdf')
                                	pdf(dir, width = x_length, height = y_length) # default width = 7, height = 7)
				# res(resolution-Pixel per Inch) required for converting pixel to inch.
                        	if(args[i+2] == 'png')
                                	png(dir, width = x_length, height = y_length, units = "in", res = 200)
                        	if(args[i+2] == 'jpg')
                                	jpeg(dir, width = x_length, height = y_length, units = "in", res = 200)
                        	if(args[i+2] == 'bmp')
                                	bmp(dir, width = x_length, height = y_length, units = "in", res = 200)
                        	if(args[i+2] == 'ps')
                                	postscript(dir, width = x_length, height = y_length)
                	}
        	}
	}		
	if(flag == FALSE){
		if(flag2 == TRUE){
			dir = paste(dir, "T-Rplots.pdf", sep="/")
			pdf(dir, width = x_length, height = y_length)
		}
		else
        		pdf('save/T-Rplots.pdf', width = x_length, height = y_length)
			#pdf('save/T-Rplots.pdf', width = x_length, height = y_length, family = "GB1")
	}
}
fun_output2 <- function(){
	args <- commandArgs(trailingOnly = TRUE)
	flag2 = FALSE
	if(length(args) != 0){
                for(i in 1:length(args)){
                        if(args[i] == '-i'){
                                flag2 = TRUE
                                name = args[i+1]
                                time = args[i+2]
                                dir = paste(name, time, sep="-")
                        }
                }
        }
        # Graph Size Option('-s')
        x_length = 7
        y_length = 7
        if(length(args) != 0){
                for(i in 1:length(args)){
                        if(args[i] == '-s'){
                                x_length = as.double(args[i+1])
                                y_length = as.double(args[i+2])
                        }
		}
        }
	if(flag2 == TRUE){
		dir = paste(dir, "T-Rplot%03d.jpg", sep="/")
                jpeg(dir, width = x_length, height = y_length, units="in", res=200)
	}
	else
		jpeg('save/T-Rplot%03d.jpg', width = x_length, height = y_length, units="in", res=200)
}
#most <- read.table("result3.po", sep='\t', fill = TRUE, stringsAsFactors=FALSE)
#colnames(most) <-c("time", "action", "RW", "sec_num", "sec_size", "p_io", "pname", "block", "file")
#write.table(most, "sample.txt")
#barplot(table(most$action))
#barplot(table(most$RW))
#barplot(table(most$sec_size))

# Function for Sector Number Graph
fun_num <- function(most,...){
	plot(sec_num~time, data=most, xlab = "time(sec)")
	xrange<-c(min(most$time),max(most$time))
	yrange<-c(min(most$sec_num), max(most$sec_num))
	# Point Option('-p')
        flag = FALSE
        args <- commandArgs(trailingOnly = TRUE)
        if(length(args) != 0){
                for(i in 1:length(args)){
                        if(args[i] == '-p'){
                                if(args[i+1] == 's')
                                        flag = TRUE
                        }
                }
        }
	if(flag == FALSE){
		plot(sec_num~time, data=most[most$block == "D",], col="blue", xlim=xrange, ylim=yrange, xlab = "time(sec)")
		par(new = T)
		plot(sec_num~time, data=most[most$block == "J",], col="red", xlim=xrange, ylim=yrange, xlab = "time(sec)")
		par(new = T)
		plot(sec_num~time, data=most[most$block == "M",], col="black", xlim=xrange, ylim=yrange, xlab = "time(sec)")
		legend("right", legend=c("Data", "Journal", "Meta"), pch=21, pt.bg=c("blue", "red", "black"))
	}
	else{
		plot(sec_num~time, data=most[most$block == "D",], pch = 1, xlim=xrange, ylim=yrange, xlab = "time(sec)")
                par(new = T)
                plot(sec_num~time, data=most[most$block == "J",], pch = 2, xlim=xrange, ylim=yrange, xlab = "time(sec)")
                par(new = T)
                plot(sec_num~time, data=most[most$block == "M",], pch = 3, xlim=xrange, ylim=yrange, xlab = "time(sec)")
                legend("right", legend=c("Data", "Journal", "Meta"), pch=c(1, 2, 3))
	}
}
# Function for Sector Size Graph
fun_size <- function(most,...){ #512byte -> 1KB
	plot(sec_size/2~time, data=most, xlab = "time(sec)", ylab = "sec_size(1KB)")
	xrange<-c(min(most$time),max(most$time))
	yrange<-c(min(most$sec_size), max(most$sec_size/2))
	# Point Option('-p')
        flag = FALSE
        args <- commandArgs(trailingOnly = TRUE)
        if(length(args) != 0){
                for(i in 1:length(args)){
                        if(args[i] == '-p'){
                                if(args[i+1] == 's')
                                        flag = TRUE
                        }
                }
        }
	if(flag == FALSE){
		plot(sec_size/2~time, data=most[most$block == "D",], col="blue", xlim=xrange, ylim=yrange, xlab = "time(sec)", ylab = "sec_size(1KB)")
		par(new = T)
		plot(sec_size/2~time, data=most[most$block == "J",], col="red", xlim=xrange, ylim=yrange, xlab = "time(sec)", ylab = "sec_size(1KB)")
		par(new = T)
		plot(sec_size/2~time, data=most[most$block == "M",], col="black", xlim=xrange, ylim=yrange, xlab = "time(sec)", ylab = "sec_size(1KB)")
		legend("topright", legend=c("Data", "Journal", "Meta"), pch=21, pt.bg=c("blue", "red", "black"))
	}
	else{
		plot(sec_size/2~time, data=most[most$block == "D",], pch=1, xlim=xrange, ylim=yrange, xlab = "time(sec)", ylab = "sec_size(1KB)")
                par(new = T)
                plot(sec_size/2~time, data=most[most$block == "J",], pch=2, xlim=xrange, ylim=yrange, xlab = "time(sec)", ylab = "sec_size(1KB)")
                par(new = T)
                plot(sec_size/2~time, data=most[most$block == "M",], pch=3, xlim=xrange, ylim=yrange, xlab = "time(sec)", ylab = "sec_size(1KB)")
                legend("topright", legend=c("Data", "Journal", "Meta"), pch=c(1, 2, 3))
	}
}
# Y-axis Option(-y)
fun_y <- function(){
	args <- commandArgs(trailingOnly = TRUE)
	flag = FALSE
	if(length(args) != 0){
        	for(i in 1:length(args)){
                	if(args[i] == '-y'){
				flag = TRUE
                        	if(args[i+1] == 's'){
                        	        #plot(sec_size~time, data=most)
					fun_size(most)
				}
                        	if(args[i+1] == 'n'){
                        	        #plot(sec_num~time, data=most)
					fun_num(most)
				}
                        	if(args[i+1] == 'all'){
                        	        #plot(sec_size~time, data=most)
					fun_size(most)
					#plot(sec_num~time, data=most)
					fun_size(most)
				}
                	}	
        	}
	}
	if(flag == FALSE){
		fun_size(most)
		fun_num(most)
		# Bar plot for sector number
		yrange_RW<-c(0,max(table(most$RW))*1.35)
		barplot(table(most$RW), xlab = "rwbs type", ylab = "I/O Count", ylim = yrange_RW)
		# Bar plot for sector size(512byte -> 1KB)
		yrange_SS<-c(0,max(table(most$sec_size/2))*1.35)
		barplot(table(most$sec_size/2), xlab = "sec_size(1KB)", ylab = "I/O Count", ylim = yrange_SS)
		hist(t(most$sec_size/2), main = "Histogram(Distribution of Sector Size)", xlab = "sec_size(1KB)", ylab = "I/O Count", breaks = seq(min(most$sec_size/2), max(most$sec_size/2), by=4))
		#plot(sec_num~time, data=most)
        	#plot(sec_size~time, data=most)
	}
}
#plot(sec_num~time, data=most)
#plot(sec_size~time, data=most)
#fun_num <- function(most,...){
#xrange<-c(0, 7)
#yrange<-c(0, 2.5e+7)
#plot(sec_num~time, data=most[most$block == "D",], col="blue", xlim=xrange, ylim=yrange)
#par(new = T)
#plot(sec_num~time, data=most[most$block == "J",], col="red", xlim=xrange, ylim=yrange)
#par(new = T)
#plot(sec_num~time, data=most[most$block == "M",], col="black", xlim=xrange, ylim=yrange)
#legend("right", legend=c("Data", "Journal", "Meta"), pch=21, pt.bg=c("blue", "red", "black"))
#}
#library(ggplot2)
#ggplot(most, aes(x=time, y=sec_num)) + geom_point(aes(colour=block))
#ggplot(most, aes(x=action)) + geom_bar(aes(fill=block))
sub_main <- function(){
	fun_output()
	fun_y()
	fun_output2()
	fun_y()
	garbage <- dev.off()
}
# main
most <- read.table("result3.po", sep='\t', fill = TRUE, stringsAsFactors=FALSE)
colnames(most) <-c("time", "action", "RW", "sec_num", "sec_size", "p_io", "pname", "block", "file")
sub_main()
