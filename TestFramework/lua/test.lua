require( "iuplua" )
require( "iupluacontrols" )
require( "iuplua_pplot"  )


qsortLegend = "Quick Sort"
qsortSteps = {} 	--imported from C++ host
qsortTime = {}		--imported from C++ host

nsortLegend = "Merge Sort"
nsortSteps = {} 	--imported from C++ host
nsortTime = {} 		--imported from C++ host

--Sorting iterator, since items layout is random
function pairsByKey(t, f)
	local a = {}
	for n in pairs(t) do table.insert(a, n) end
	table.sort(a,f)
	local i = 0
	local iter = function ()
		i = i + 1
		if a[i] == nil then return nil
		else return a[i], t[a[i]]
		end
	end
	return iter
end

--Interface exported to C++ host
function plot()
-- Create Steps plotter
plot_steps =iup.pplot{TITLE = "Steps Comparison",
                    MARGINBOTTOM="55",
                    MARGINLEFT="75",
                    AXS_XLABEL="Items in the array",
                    AXS_YLABEL="Operation Cost(steps)",
					AXS_YAUTOMIN = "YES",
					AXS_XAUTOMIN = "YES",
					AXS_YAUTOMAX = "YES",
					AXS_XAUTOMAX = "YES",
					LEGENDSHOW = "YES",
                    ["USE_GDI+"] = "YES",
					}
iup.PPlotBegin(plot_steps,0)
for k,v in pairsByKey(qsortSteps) do
	iup.PPlotAdd(plot_steps, k, v)
end
iup.PPlotEnd(plot_steps)
plot_steps.DS_MODE = "MARKLINE"
plot_steps.DS_MARKSTYLE = "CIRCLE"
plot_steps.DS_LEGEND = qsortLegend


iup.PPlotBegin(plot_steps,0)
for k,v in pairsByKey(nsortSteps) do
	iup.PPlotAdd(plot_steps, k, v)
end
iup.PPlotEnd(plot_steps)
plot_steps.DS_MODE = "MARKLINE"
plot_steps.DS_MARKSTYLE = "CIRCLE"
plot_steps.DS_LEGEND = nsortLegend

--Create Time consumation plotter
plot_time =iup.pplot{TITLE = "Run Time Comparison",
                    MARGINBOTTOM="55",
                    MARGINLEFT="75",
                    AXS_XLABEL="Items in the array",
                    AXS_YLABEL="Time Cost(ms)",
					AXS_YAUTOMIN = "YES",
					AXS_XAUTOMIN = "YES",
					AXS_YAUTOMAX = "YES",
					AXS_XAUTOMAX = "YES",
					LEGENDSHOW = "YES",
                    ["USE_GDI+"] = "YES",
					}
iup.PPlotBegin(plot_time,0)
for k,v in pairsByKey(qsortTime) do
	--print(qsort.ArraySize[i].." k",  qsort.Steps[i])
	iup.PPlotAdd(plot_time, k, v)
end
iup.PPlotEnd(plot_time)
plot_time.DS_MODE = "MARKLINE"
plot_time.DS_MARKSTYLE = "CIRCLE"
plot_time.DS_LEGEND = qsortLegend


iup.PPlotBegin(plot_time,0)
for k,v in pairsByKey(nsortTime) do
	--print(nsort.ArraySize[i].." k",  nsort.Steps[i])
	iup.PPlotAdd(plot_time, k, v)
end
iup.PPlotEnd(plot_time)
plot_time.DS_MODE = "MARKLINE"
plot_time.DS_MARKSTYLE = "CIRCLE"
plot_time.DS_LEGEND = nsortLegend


--Create the dialog
vbox = iup.vbox{plot_steps, plot_time, gap = 5}
dlg = iup.dialog{vbox; title="A Simple Profiler [By Coder_In_Zen_Mode]",size="THIRDxHALF"}

dlg:show()

iup.MainLoop()
end

