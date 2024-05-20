using Microsoft.AspNetCore.Mvc;

namespace SimpleAPI.Controllers
{
    public class HomeController : Controller
    {
        public IActionResult Index()
        {
            return View();
        }
    }
}