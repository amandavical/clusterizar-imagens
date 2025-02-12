# Trabalho Final de Clusterização de Imagens
---

![About us](https://github.com/Destiny-Corporation/.github/assets/93869144/86fe81b6-882b-4755-8eee-3e84740473b2)


### ✦ | About Us ⚡
*We are a team of second-semester Computer Science students at IFCE, currently working on an image clustering algorithm. In this project, we manipulate vectors and apply programming and data analysis techniques to explore patterns and group images effectively. This hands-on experience helps us bridge theoretical knowledge with practical applications.*

*(We are open to new contributors)*

---

![Destiny's Childs](https://github.com/Destiny-Corporation/.github/assets/93869144/5d77cdf7-da34-44d3-8c02-afe7e0b58c0b)

### ✦ | Team:
<div align="center">
<table>
<tbody>
<tr>
<br>
<td align="center"><a href="https://github.com/amandavical"><img src="https://avatars.githubusercontent.com/u/95192936?v=4" width="100px;" alt="Amanda Vieira"/><br /><sub><b>Amanda Vieira</b></sub></a><br /></td>
<td align="center"><a href="https://github.com/yolcastro"><img src="https://avatars.githubusercontent.com/u/176954163?v=4" width="100px;" alt="Yolanda Castro"/><br /><sub><b>Yolanda Castro</b></sub></a><br /></td>
<td align="center"><a href="https://github.com/DerickBessa"><img src="https://avatars.githubusercontent.com/u/159495182?v=4" width="100px;" alt="Derick Bessa"/><br /><sub><b>Derick Bessa</b></sub></a><br /></td>
<td align="center"><a href="https://github.com/souza-rubens"><img src="https://avatars.githubusercontent.com/u/99306045?v=4" width="100px;" alt="Rubens Souza"/><br /><sub><b>Rubens Souza</b></sub></a><br /></td>
</tr>
</tr>
</tbody>
</table>
</div>

---

![infinite](https://github.com/Destiny-Corporation/.github/assets/93869144/e639d368-461d-4276-b2ec-3cefd40bd678)


<h3 align="center"> » Connect with us:</h3>
<div align="center">

<a href="mailto:amandaestudos25@@gmail.com"><img src="https://img.shields.io/badge/-Gmail-%23333?style=for-the-badge&logo=gmail&logoColor=white" target="_blank"></a>  
</div>

### ✦ | About our project⚡:

# K-means Image Simplification

A C project that uses the K-means algorithm to simplify images by clustering pixel colors.

## Table of Contents

- [Introduction](#introduction)
- [Setup](#setup)
- [Features](#features)
- [File Structure](#file-structure)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Introduction

K-means Image Simplification is a project that demonstrates how the K-means clustering algorithm can be used to simplify images by reducing the number of unique colors. This is done by grouping pixels with similar colors into clusters and replacing all pixels in each cluster with the centroid color of the cluster. This technique is commonly used in image compression and simplification tasks.

## Setup

1. Clone the repository:
   bash
 https://github.com/amandavical/clusterizar-imagens.git  
 

2. Install the required dependencies (if any):  
   This project does not have external dependencies.

3. Compile the project using the provided Makefile:
   bash
   make
   

4. The program uses PGM format images. Ensure that the input image is in the correct format.

## Features

Here are the main features and functionality available in this project:

- K-means Clustering: The core functionality of the project is the implementation of the K-means algorithm, which is used to simplify the colors in an image by grouping similar colors together.
- Image Simplification: After the K-means algorithm clusters the colors in the image, the pixels are replaced with the centroid color of their respective clusters, simplifying the image.
- PGM Image Format Support: The program reads and writes images in the PGM format, which is a simple grayscale image format.
- Interactive Execution: The program allows you to specify the number of clusters (k) and the maximum number of iterations for the algorithm, providing control over the simplification process.

## File Structure

The project follows this directory structure:

bash
kmeans-image-simplification/
│
├── src/
│   ├── kmeans.c
│   ├── kmeans.h
│   ├── pgm.c
│   ├── pgm.h
│   ├── utils.c
│   └── utils.h
│
├── Makefile
├── README.md
└── input_image.pgm


- src/: Contains all source files for the program.
- kmeans.c and kmeans.h: Implements the K-means algorithm and defines its interface.
- pgm.c and pgm.h: Handles reading and writing of PGM image files.
- utils.c and utils.h: Contains utility functions for handling basic operations like distance calculations.

## Usage

1. Compile the project using the following command:

   - make 

2. Clean all "trash"

  - make clean
 
3. The program will generate a simplified version of the image with reduced colors.

## Contributing

Contributions are welcome! If you find any issues or want to add new features, feel free to submit a pull request.

## License

This project is licensed under the MIT License.

