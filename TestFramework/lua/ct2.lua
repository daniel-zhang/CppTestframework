function f1(t)
	local p = t
	function f2()
		print(p.." f2 here")
	end
	function f3()
		p = p.."+1"
		print(p.." f3 here")
	end
	function f4()
		print(p.." f4 here")
		return p
	end
end

function add(s)

	for k,v in pairs(s) do
		print(k.."="..v)
	end

end
--main chunk
rt = {}

f1("start_1")
f2()
f3(); f3(); f3()
table.insert(rt, f4())

f1("start_2")
f2()
f3(); f3(); f3()
table.insert(rt, f4())

add(rt)



