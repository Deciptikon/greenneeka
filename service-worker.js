const APP_VERSION = "1.2.0";
const APP_NAME = "greenneeka";

const CACHE_NAME =
  `greenneeka-${APP_VERSION}` + new Date().toISOString().split("T")[0];
const basePath = self.location.pathname.startsWith(`/${APP_NAME}`)
  ? `/${APP_NAME}`
  : self.location.pathname.replace(/\/[^/]*$/, "");
const pwa = "/pwa";
console.log(`basePath: ${basePath}`);

self.addEventListener("install", (event) => {
  console.log("SW install");

  event.waitUntil(
    caches.open(CACHE_NAME).then((cache) => {
      return cache.addAll([
        `${basePath}/`,
        `${basePath}/favicon.ico`,
        `${basePath}${pwa}/css/styles.css`,
        `${basePath}${pwa}/js/app.js`,
        `${basePath}${pwa}/images/logo-512x512.png`,
        `${basePath}${pwa}/images/icon-192x192.png`,
        `${basePath}${pwa}/images/icon-512x512.png`,
        `${basePath}${pwa}/images/screenshot-wide.png`,
        `${basePath}${pwa}/images/screenshot-mobile.png`,
      ]);
    })
  );
});

self.addEventListener("fetch", (event) => {
  event.respondWith(
    caches.match(event.request).then((response) => {
      return (
        response ||
        fetch(event.request).catch((error) => {
          console.error("Fetch failed:", error);
          return new Response("Network error occurred", { status: 408 });
        })
      );
    })
  );
});

self.addEventListener("activate", (event) => {
  event.waitUntil(
    caches.keys().then((cacheNames) => {
      return Promise.all(
        cacheNames.map((cacheName) => {
          if (cacheName !== CACHE_NAME) {
            console.log(`Deleting cache: ${cacheName}`);
            return caches.delete(cacheName);
          }
        })
      );
    })
  );
});
