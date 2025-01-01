# Corydon
![GitHub License](https://img.shields.io/github/license/boprr/corydon)
![GitHub Release](https://img.shields.io/github/v/release/boprr/corydon)
![GitHub contributors](https://img.shields.io/github/contributors-anon/boprr/corydon)
![GitHub repo size](https://img.shields.io/github/repo-size/boprr/corydon)
![GitHub commit activity](https://img.shields.io/github/commit-activity/m/boprr/corydon)
![GitHub top language](https://img.shields.io/github/languages/top/boprr/corydon)

## Building
### Requirements
- cmake
- wget
- tar
- git
- xorriso
###### (gcc toolchain)
- system gcc/g++
- make
- bison
- flex
- gmp
- mpfr
- mpc
- texinfo

# GNU/Linux
### gentoo
```
emerge -a gcc make bison flex gmp mpc mpfr texinfo isl cmake wget tar git libburn
```
### arch
```
pacman -S base-devel gmp libmpc mpfr wget tar git libburn
```

## Contributing 
1. Fork the repository.
2. Create a new branch: `git checkout -b feature-name`.
3. Make your changes.
4. Push your branch: `git push origin feature-name`.
5. Format your code with clang-format
6. Create a pull request.

## License
This project is licensed under the GNU General Public License v3.0. See the [LICENSE](LICENSE) file for details.
