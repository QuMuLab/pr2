
FROM ubuntu:20.04

#maintainer information
LABEL maintainer="Christian Muise (christian.muise@queensu.ca)"

# update the apt package manager
RUN apt-get update
RUN apt-get install -y software-properties-common
RUN apt-get update && apt-get -y install locales

# install common packages
RUN apt-get install -y \
        build-essential \
        cmake \
        make \
        g++ \
        vim \
        git \
        bison \
        flex \
        dos2unix \
        graphviz \
        time \
        bc

RUN DEBIAN_FRONTEND=noninteractive apt-get install -y expect

# Grab java for paladinus
RUN add-apt-repository -y ppa:linuxuprising/java
RUN apt-get update
RUN echo oracle-java17-installer shared/accepted-oracle-license-v1-3 select true | /usr/bin/debconf-set-selections
RUN apt-get install -y oracle-java17-installer

# Grab a different java for MyND
RUN apt-get install -y openjdk-8-jdk maven
RUN echo "export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64/" >> ~/.mavenrc

# install python and related
RUN apt-get install -y python3 python3-dev python3-pip python3-venv python-is-python3
RUN apt-get install -y python2
RUN pip3 install --upgrade pip
RUN pip3 install --upgrade graphviz
RUN pip3 install --upgrade networkx
RUN pip3 install --upgrade pydot


# Grab the other planners
WORKDIR /PLANNERS

RUN git clone https://github.com/tomsons22/FOND-SAT.git fondsat

# Using personal fork of MyND
#RUN git clone https://github.com/robertmattmueller/myND.git mynd
RUN git clone https://github.com/haz/myND.git mynd
RUN cd mynd && ./build.sh

RUN git clone https://github.com/ramonpereira/paladinus.git

RUN git clone https://github.com/QuMuLab/planner-for-relevant-policies.git prp
RUN cd prp/src && ./build_all -j 8

RUN mkdir /PLANNERS/bin

RUN echo "#!/bin/bash" >> /PLANNERS/bin/fondsat
RUN echo "python /PLANNERS/fondsat/src/main.py --mem_limit 4096 --solver glucose \$@" >> /PLANNERS/bin/fondsat

RUN echo "#!/bin/bash" >> /PLANNERS/bin/mynd
RUN echo "ln -s /PLANNERS/mynd/translator-fond translator-fond" >> /PLANNERS/bin/mynd
RUN echo "java -Xmx4g -cp /PLANNERS/mynd/target/mynd-1.0-SNAPSHOT.jar mynd.MyNDPlanner -type FOND -search LAOSTAR -heuristic FF \$@" >> /PLANNERS/bin/mynd

RUN echo "#!/bin/bash" >> /PLANNERS/bin/paladinus
RUN echo "ln -s /PLANNERS/paladinus/translator-fond/ translator-fond" >> /PLANNERS/bin/paladinus
RUN echo "java -jar -Xmx4g /PLANNERS/paladinus/paladinus1.0.jar -search ITERATIVE_DFS_PRUNING -heuristic HADD -actionSelectionCriterion MIN_MAX_H -evaluationFunctionCriterion MAX \$@ -timeout 3600" >> /PLANNERS/bin/paladinus

RUN echo "#!/bin/bash" >> /PLANNERS/bin/prp
RUN echo "/PLANNERS/prp/src/prp \$@" >> /PLANNERS/bin/prp

RUN echo "#!/bin/bash" >> /PLANNERS/bin/pr2
RUN echo "/PROJECT/pr2 \$@" >> /PLANNERS/bin/pr2

# Update the PATH variable and permissions
ENV PATH="/PLANNERS/bin:${PATH}"
RUN chmod -R 777 /PLANNERS/bin

WORKDIR /PROJECT

# default command to execute when container starts
CMD /bin/bash
