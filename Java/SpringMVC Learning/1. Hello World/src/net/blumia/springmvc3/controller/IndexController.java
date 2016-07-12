package net.blumia.springmvc3.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;

// 告诉spring mvc: 这个类是个控制器，或者说 这个类接受(HTTP)请求。
@Controller
// 告诉spring mvc: 这个类处理这个路径下面的请求。
@RequestMapping("/blumia/")
public class IndexController {
	// 不写参数的 @RequestMapping 就处理 "/blumia/" 下面的任意请求。// 稍后我再看看具体情况.
	@RequestMapping// () 写不写都行 如果不填写参数的话.
	public String _index() {
		// same as "/WEB-INF/views/index.jsp", but spring do the job.
		return "index";
	}

	@RequestMapping("/test.do")
	public String test() {
		return "test";
	}
}
