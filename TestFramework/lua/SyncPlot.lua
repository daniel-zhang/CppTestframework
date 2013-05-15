require( "iuplua" )
require( "iupluacontrols" )
require( "iuplua_pplot"  )

function sortIndex(t,f)
	local tmp = {}
	for n in pairs(t) do
		table.insert(tmp, n)
	end
	table.sort(tmp,f)
	local i = 0
	local iter = function()
		i = i + 1
		if tmp[i] ==nil then return nil --boundary check
		else return tmp[i], t[tmp[i]] end
	end
	return iter
end

function createPlot(tb)
	local plot = iup.pplot{TITLE = tb.title,
                    MARGINBOTTOM="55",
                    MARGINLEFT="75",
                    AXS_XLABEL=tb.xlabel,
                    AXS_YLABEL=tb.ylabel,
					AXS_YAUTOMIN = "YES",
					AXS_XAUTOMIN = "YES",
					AXS_YAUTOMAX = "YES",
					AXS_XAUTOMAX = "YES",
					LEGENDSHOW = "YES",
                    ["USE_GDI+"] = "YES",
					}

	function addDataSet(plot)
		iup.PPlotBegin(plot, 0)
		function add(x,y)
			iup.PPlotAdd(plot,x,y)
		end
		iup.PPlotEnd(plot)
		plot.DS_MODE = "MARKLINE"
		plot.REDRAW = "YES"
	end
	dlg = iup.dialog{plot,title = tb.title, size = "HALFxHALF"}
	dlg:show()
	iup.MainLoop()
end

createPlot{xlabel = "x", ylabel = "y", title = "hello closure"}
add(2,3)
add(4,1)
