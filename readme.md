#%setvar title textblock macro processor
#%include template.txb

# `textblock` macro processor
Textblock is an extensible  macro processor for unix-like systems that permits
shell command execution.  Primarily created to process some section of your text with familliar unix utilities, like column, fmt or even include a gnuplot figure..

## Examples
Filtering through external program:

	#%block filter [cmd]
		[text]
	#%end

File inclusion

	#%include [filename]

Quoting

```
#%q q block filter echo hello
#%q q end
```

Swapping the files while processing

```
<tag>
#%q swap
</tag>
```
