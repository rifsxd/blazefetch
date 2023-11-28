<div align="center">
	<h1>Blazefetch</h1>
	<blockquote align="center">⚡ A lite & blazing fast system info fetch utility.</blockquote>
	<p><br>
		⚡ Blazefetch is a lite & blazing fast tool for fetching info about your system.
	</p><br>
	<p><br> Iconic</p><br>
	<img src="/assets/iconic_prev.png">
	<p><br> Legacy</p><br>
	<img src="/assets/legacy_prev.png">
</div>

# Install
Binaries are provided at the releases page [here](https://github.com/rifsxd/blazefetch/releases).

## Manual Compile
```sh
git clone https://github.com/rifsxd/blazefetch
cd blazefetch/src
sudo make clean install
```
 - For just icons as headers
```sh
git clone https://github.com/rifsxd/blazefetch
cd blazefetch/src
sudo make clean install_iconic
```  

# Usage
Run `blazefetch` with `-d` or `--daemon`  flag to initialize the daemon process once then run `blazefetch` again to get the fetch info working, if you kill the blaze daemon then blazefetch wont work. Also `-v` or `--version` to get version info.

# License
Blazefetch is licensed under the MIT license.  
[Read here](LICENSE) for more info.
